#include "customer.h"
#include "ui_customer.h"
#include "mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>

Customer::Customer(QWidget *parent, MainWindow *mainWindow) :
    QWidget(parent),
    ui(new Ui::Customer),
    mainWindow(mainWindow)
{
    ui->setupUi(this);

    //connect(ui->pushButtonAddCustomer, &QPushButton::clicked, this, &Customer::on_pushButtonAddCustomer_clicked);
    connect(ui->pushButtonEditCustomer, &QPushButton::clicked, this, &Customer::on_pushButtonEditCustomer_clicked);
    connect(ui->pushButtonRemoveCustomer, &QPushButton::clicked, this, &Customer::on_pushButtonRemoveCustomer_clicked);
    //connect(ui->pushButtonBack, &QPushButton::clicked, this, &Customer::on_pushButtonBack_clicked);

    if (connect_sqlite_database()) {
        load_sqlite_database(ui->tableViewCustomer, "customer");
    } else {
        QMessageBox::information(this, "Error", "Database cannot be opened!");
    }
}

Customer::~Customer()
{
    delete ui;
}

bool Customer::connect_sqlite_database() {
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.setDatabaseName("rental.db");
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

void Customer::load_sqlite_database(QTableView *tableView, const QString &tableName) {

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    QSqlQueryModel *reflecting_model = new QSqlQueryModel();
    QSqlQuery query;
    query.exec("SELECT * FROM " + tableName);
    reflecting_model->setQuery(std::move(query)); // std::move ile değiştirdik
    tableView->setModel(reflecting_model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableView->verticalHeader()->setVisible(false);
}

void Customer::on_pushButtonAddCustomer_clicked() {

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    if(db.open()){
    QString customer_id = ui->lineEditCustomerId->text();
    QString name = ui->lineEditCustomerName->text();
    QString telNum = ui->lineEditCustomerPhoneNumber->text();
    QString email = ui->lineEditEmail->text();
    QString address = ui->lineEditAdress->text();

    QSqlQuery query;
    query.prepare("INSERT INTO customer (id, name, telNum, email, address) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(customer_id);
    query.addBindValue(name);
    query.addBindValue(telNum);
    query.addBindValue(email);
    query.addBindValue(address);

    if(!query.exec()) {
        ui->labelResultCustomer->setText("Customer is not added, customer id is already registered in the system.");
    } else {
        ui->labelResultCustomer->setText("Customer is added successfully");
        load_sqlite_database(ui->tableViewCustomer, "customer"); // Tabloyu güncelle
    }
    }
    else{
        ui->labelResultCustomer->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();

    }
}

void Customer::on_pushButtonEditCustomer_clicked() {

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    if (db.open()) {
        QString customer_id = ui->lineEditCustomerId->text();
        QString name = ui->lineEditCustomerName->text();
        QString telNum = ui->lineEditCustomerPhoneNumber->text();  // Düzeltme: email yerine telNum
        QString email = ui->lineEditEmail->text();    // Düzeltme: address yerine email
        QString address = ui->lineEditAdress->text();

        QSqlQuery query;
        query.prepare("UPDATE customer SET name = ?, telNum = ?, email = ?, address = ? WHERE id = ?");
        query.addBindValue(name);
        query.addBindValue(telNum);
        query.addBindValue(email);
        query.addBindValue(address);
        query.addBindValue(customer_id);

        if (!query.exec()) {
            qDebug() << "Query Error: " << query.lastError().text();
            ui->labelResultCustomer->setText("Customer is not edited, please check the information.");
        } else {
            ui->labelResultCustomer->setText("Customer is edited successfully");
            load_sqlite_database(ui->tableViewCustomer, "customer"); // Tabloyu güncelle
        }
    } else {
        ui->labelResultCustomer->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();
    }
}

void Customer::on_pushButtonRemoveCustomer_clicked() {

    QSqlDatabase db = QSqlDatabase::database("qt_sql_default_connection");
    db.setDatabaseName("/Users/omer/Car_rental/rental.db");

    if(db.open()){
    QString customer_id = ui->lineEditCustomerId->text();

    QSqlQuery query;
    query.prepare("DELETE FROM customer WHERE id = ?");
    query.addBindValue(customer_id);

    if(!query.exec()) {
        qDebug() << "Query Error: " << query.lastError().text();
        ui->labelResultCustomer->setText("Customer is not removed, please check the information.");
    } else {
        ui->labelResultCustomer->setText("Customer is removed successfully");
        load_sqlite_database(ui->tableViewCustomer, "customer"); // Tabloyu güncelle
    }
    }
    else{
        ui->labelResultCustomer->setText("Database is not connected");
        qWarning() << "Error: " << db.lastError().text();

    }
}


void Customer::on_pushButtonBack_clicked() {
    qDebug() << "Back button clicked";
    MainWindow *mainWindowWidget = new MainWindow();
    mainWindowWidget->show();
    this->close();  // Close the current Car widget
}
