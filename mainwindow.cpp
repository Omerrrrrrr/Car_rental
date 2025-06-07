#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "customer.h"
#include "car.h"
#include "rentacar.h"


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QJsonDocument>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QTableWidgetItem>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    customerWidget = new Customer();
    carWidget = new Car();
    rentacarWidget = new Rentacar();



    // connect(ui->pushButtonCars, &QPushButton::clicked, this, &MainWindow::on_pushButtonCars_clicked);
    // connect(ui->pushButtonCustomers, &QPushButton::clicked, this, &MainWindow::on_pushButtonCustomers_clicked);
    // connect(ui->pushButtonRentacar, &QPushButton::clicked, this, &MainWindow::on_pushButtonRentacar_clicked);
    // connect(ui->pushButtonExport, &QPushButton::clicked, this, &MainWindow::on_pushButtonExport_clicked);
    // connect(ui->pushButtonImport, &QPushButton::clicked, this, &MainWindow::on_pushButtonImport_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete customerWidget;
    delete carWidget;
    delete rentacarWidget;
}

QJsonArray MainWindow::collect_car()
{
    QJsonArray car_array;
    QSqlQuery query("SELECT * FROM car");
    while (query.next())
    {
        QJsonObject car;
        car["id"] = query.value("id").toInt();
        car["registration_number"] = query.value("registration_number").toString();
        car["brand"] = query.value("brand").toString();
        car["model"] = query.value("model").toString();
        car["modYear"] = query.value("modYear").toString();
        car["gear"] = query.value("gear").toString();
        car["fuelType"] = query.value("fuelType").toString();
        car["price"] = query.value("price").toInt();
        car["status"] = query.value("status").toString();
        car_array.append(car);
    }
    return car_array;
}

QJsonArray MainWindow::collect_customer()
{
    QJsonArray customer_array;
    QSqlQuery query("SELECT * FROM customer");
    while (query.next())
    {
        QJsonObject customer;
        customer["id"] = query.value("id").toInt();
        customer["name"] = query.value("name").toString();
        customer["telNum"] = query.value("telNum").toString();
        customer["email"] = query.value("email").toString();
        customer["adress"] = query.value("address").toString();
        customer_array.append(customer);
    }
    return customer_array;
}

void MainWindow::insert_car_json(const QJsonArray &car_array)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO car (id, registration_number, brand, model, modYear, gear, fuelType, price, status) "
                  "VALUES (:id, :registration_number, :brand, :model, :modYear, :gear, :fuelType, :price, :status) "
                  "ON DUPLICATE KEY UPDATE registration_number = :registration_number, brand = :brand, model = :model, modYear = :modYear, gear = :gear, fuel_type = :fuel_type, price = :price, status = :status");
    for (const QJsonValue &value_car : car_array)
    {
        QJsonObject car = value_car.toObject();
        query.bindValue(":id", car["id"].toInt());
        query.bindValue(":registration_number", car["registration_number"].toString());
        query.bindValue(":brand", car["brand"].toString());
        query.bindValue(":model", car["model"].toString());
        query.bindValue(":modYear", car["modYear"].toString());
        query.bindValue(":gear", car["gear"].toString());
        query.bindValue(":fuelType", car["fuelType"].toString());
        query.bindValue(":price", car["price"].toInt());
        query.bindValue(":status", car["status"].toString());

        if (!query.exec())
        {
            qDebug() << "Error inserting car:" << query.lastError();
        }
    }
    QSqlDatabase::database().commit();
}

void MainWindow::insert_customer_json(const QJsonArray &customer_array)
{
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("INSERT INTO customer (id, name, telNum, email, address) "
                  "VALUES (:id, :name, :telNum, :email, :address) "
                  "ON DUPLICATE KEY UPDATE name = :name, telNum = :telNum, email = :email, address = :address");
    for (const QJsonValue &value_customer : customer_array)
    {
        QJsonObject customer = value_customer.toObject();
        query.bindValue(":id", customer["id"].toInt());
        query.bindValue(":name", customer["name"].toString());
        query.bindValue(":telNum", customer["telNum"].toString());
        query.bindValue(":email", customer["email"].toString());
        query.bindValue(":address", customer["adress"].toString());
        if (!query.exec())
        {
            qDebug() << "Error inserting customer:" << query.lastError();
        }
    }
    QSqlDatabase::database().commit();
}

void MainWindow::export_json_file(const QString &json_file_name)
{
    QJsonObject root;

    QJsonArray car_array = collect_car();
    QJsonArray customer_array = collect_customer();

    root["cars"] = car_array; //  car_array kullanılmalı.
    root["customers"] = customer_array; //  customer_array kullanılmalı.

    QFile file(json_file_name);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(QJsonDocument(root).toJson());
        file.close();
    }
}

void MainWindow::import_json_file(const QString &json_file_name)
{
    QFile file(json_file_name);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open JSON file.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument json_document = QJsonDocument::fromJson(data);

    QJsonObject root = json_document.object();

    QJsonArray car_array = root["cars"].toArray();
    insert_car_json(car_array);

    QJsonArray customer_array = root["customers"].toArray();
    insert_customer_json(customer_array);

    ui->labelJson->hide();
    loadTableWithJsondata(car_array, customer_array);
}

void MainWindow::loadTableWithJsondata(const QJsonArray &car_array, const QJsonArray &customer_array)
{
    ui->tableWidget->clear();

    ui->tableWidget->setColumnCount(14);
    QStringList headers = {"Car ID", "Regist. Number", "Brand", "Model", "ModYear", "Gear", "Fuel Type", "Price", "Status", "Customer ID", "Name", "TelNum", "Email", "Adress"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row_count = std::max(car_array.size(), customer_array.size());
    ui->tableWidget->setRowCount(row_count);
    ui->tableWidget->verticalHeader()->setVisible(false);

    for (int i{0}; i < car_array.size(); ++i)
    {
        QJsonObject car = car_array[i].toObject();
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(car["id"].toInt())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(car["registration_number"].toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(car["brand"].toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(car["model"].toString()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(car["modYear"].toString()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(car["gear"].toString()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(car["fuelType"].toString()));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(car["price"].toInt())));
        ui->tableWidget->setItem(i, 8, new QTableWidgetItem(car["status"].toString()));
    }

    for (int i{0}; i < customer_array.size(); ++i)
    {
        QJsonObject customer = customer_array[i].toObject();
        ui->tableWidget->setItem(i, 9, new QTableWidgetItem(QString::number(customer["id"].toInt())));
        ui->tableWidget->setItem(i, 10, new QTableWidgetItem(customer["name"].toString()));
        ui->tableWidget->setItem(i, 11, new QTableWidgetItem(customer["telNum"].toString()));
        ui->tableWidget->setItem(i, 12, new QTableWidgetItem(customer["email"].toString()));
        ui->tableWidget->setItem(i, 13, new QTableWidgetItem(customer["adress"].toString()));
    }
}

void MainWindow::on_pushButtonCars_clicked() {
    qDebug() << "Cars button clicked";
    Car *carWidget = new Car(nullptr, this);
    carWidget->show();
    this->hide();  // Hide the current MainWindow
}

void MainWindow::on_pushButtonCustomers_clicked() {
    qDebug() << "Customers button clicked";
    Customer *customerWidget = new Customer();
    customerWidget->show();
    this->hide();  // Hide the current MainWindow
}

void MainWindow::on_pushButtonRentacar_clicked() {
    qDebug() << "Rent a car button clicked";
    Rentacar *rentacarWidget = new Rentacar();
    rentacarWidget->show();
    this->hide();  // Hide the current MainWindow
}


void MainWindow::on_pushButtonExport_clicked()
{
    qDebug() << "Import Data button clicked";
    QString json_file_name = QFileDialog::getSaveFileName(this, tr("Export Data"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (!json_file_name.isEmpty())
    {
        export_json_file(json_file_name);
    }
}

void MainWindow::on_pushButtonImport_clicked()
{
    qDebug() << "Export Data button clicked";
    QString json_file_name = QFileDialog::getOpenFileName(this, tr("Import Data"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (!json_file_name.isEmpty())
    {
        import_json_file(json_file_name);
    }
}
