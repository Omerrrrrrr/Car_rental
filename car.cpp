#include "car.h"
#include "ui_car.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>



Car::Car(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::Car),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    //connect(ui->pushButtonAddCar, &QPushButton::clicked, this, &Car::on_pushButtonAddCar_clicked);
    connect(ui->pushButtonEditCar, &QPushButton::clicked, this, &Car::on_pushButtonEditCar_clicked);
    connect(ui->pushButtonRemoveCar, &QPushButton::clicked, this, &Car::on_pushButtonRemoveCar_clicked);
    connect(ui->pushButtonBack, &QPushButton::clicked, this, &Car::on_pushButtonBack_clicked);

    if (connect_sqlite_database()) {
        load_sqlite_database(ui->tableViewCar, "car");
    } else {
        QMessageBox::information(this, "Error", "Database cannot be opened!");
    }
}

Car::~Car()
{
    delete ui;
}

bool Car::connect_sqlite_database() {
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName("/Users/omer/Car_rental/rental.db");
    } else {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }

    if (!db.open()) {
        qDebug() << "Database connection problem!";
        qDebug() << "Error: " << db.lastError().text();
        return false;
    }

    return true;
}


void Car::load_sqlite_database(QTableView *tableView, const QString &tableName) {
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    QSqlQueryModel *reflecting_model = new QSqlQueryModel();
    QSqlQuery query;
    query.exec("SELECT * FROM " + tableName);
    reflecting_model->setQuery(std::move(query));
    tableView->setModel(reflecting_model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->verticalHeader()->setVisible(false);
}

void Car::on_pushButtonAddCar_clicked() {
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    if(db.open()){
        QString id = ui->lineEditCarId->text();
        QString registration_number = ui->lineEditRegistrationNumber->text();
        QString brand = ui->lineEditBrand->text();
        QString model = ui->lineEditModel->text();
        int modYear = ui->lineEditProductionYear->text().toInt();
        QString gear = ui->lineEditGear->text();
        QString fuelType = ui->lineEditFuelType->text();
        int price = ui->lineEditPrice->text().toInt();
        QString status = ui->lineEditStatus->text();

        QSqlQuery query(db);
        qDebug() << "Query Error1: " << query.lastError().text();   // tekrar kontrol et..!!!!!!

        query.prepare("INSERT INTO car (id, registration_number, brand, model, modYear, gear, fuelType, price, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(id);
        query.addBindValue(registration_number);
        query.addBindValue(brand);
        query.addBindValue(model);
        query.addBindValue(modYear);
        query.addBindValue(gear);
        query.addBindValue(fuelType);
        query.addBindValue(price);
        query.addBindValue(status);

        if(!query.exec()) {
            qDebug() << "Query Error: " << query.lastError().text();
            qDebug() << "Last Query: " << query.lastQuery();
            ui->labelResult->setText("Car is not added, registration number or car id is already registered in the system.");
        } else {
            ui->labelResult->setText("Car is added successfully");
            load_sqlite_database(ui->tableViewCar, "car"); // Tabloyu güncelle
        }
    }
    else{
        ui->labelResult->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();

    }
}

void Car::on_pushButtonEditCar_clicked() {
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    if(db.open()){
        QString id = ui->lineEditCarId->text();
        QString registration_number = ui->lineEditRegistrationNumber->text();
        QString brand = ui->lineEditBrand->text();
        QString model = ui->lineEditModel->text();
        int modYear = ui->lineEditProductionYear->text().toInt();
        QString gear = ui->lineEditGear->text();
        QString fuelType = ui->lineEditFuelType->text();
        int price = ui->lineEditPrice->text().toInt();
        QString status = ui->lineEditStatus->text();

        QSqlQuery query(db);
        query.prepare("UPDATE car SET registration_number = ?, brand = ?, model = ?, modYear = ?, gear = ?, fuelType = ?, price = ?, status = ? WHERE id = ?");
        query.addBindValue(registration_number);
        query.addBindValue(brand);
        query.addBindValue(model);
        query.addBindValue(modYear);
        query.addBindValue(gear);
        query.addBindValue(fuelType);
        query.addBindValue(price);
        query.addBindValue(status);
        query.addBindValue(id);

        if(!query.exec()) {
            qDebug() << "Query Error: " << query.lastError().text();
            ui->labelResult->setText("Car is not edited, please check the information.");
        } else {
            ui->labelResult->setText("Car is edited successfully");
            load_sqlite_database(ui->tableViewCar, "car"); // Tabloyu güncelle
        }
    }
    else{
        ui->labelResult->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();
        return;
    }
}

void Car::on_pushButtonRemoveCar_clicked() {
    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    if(db.open()){
        QString id = ui->lineEditCarId->text();

        QSqlQuery query(db);
        query.prepare("DELETE FROM car WHERE id = ?");
        query.addBindValue(id);

        if(!query.exec()) {
            qDebug() << "Query Error: " << query.lastError().text();
            ui->labelResult->setText("Car is not removed, please check the information.");
        } else {
            ui->labelResult->setText("Car is removed successfully");
            load_sqlite_database(ui->tableViewCar, "car"); // Tabloyu güncelle
        }
    }
    else{
        ui->labelResult->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();

    }
}

void Car::on_pushButtonBack_clicked() {
    qDebug() << "Back button clicked";
    if (mainWindow) {
        mainWindow->show();
    }
    this->close();  // Close the current Car widget
}
