#include "rentacar.h"
#include "mainwindow.h"
#include "QtSql/qsqlrecord.h"
#include "ui_rentacar.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>

Rentacar::Rentacar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Rentacar)
{
    ui->setupUi(this);

    // Connect slots to UI elements
    connect(ui->pushButtonAssigningCarToCustomer, &QPushButton::clicked, this, &Rentacar::on_pushButtonAssigningCarToCustomer_clicked);
    connect(ui->pushButtonUnassigningAgreement, &QPushButton::clicked, this, &Rentacar::on_pushButtonUnassigningAgreement_clicked);
    connect(ui->pushButtonSearchCar_2, &QPushButton::clicked, this, &Rentacar::on_pushButtonSearchCar_clicked);
    connect(ui->pushButtonSearchCustomer_2, &QPushButton::clicked, this, &Rentacar::on_pushButtonSearchCustomer_clicked);
    connect(ui->pushButtonRefreshCarTable, &QPushButton::clicked, this, &Rentacar::on_pushButtonRefreshCarTable_clicked);
    connect(ui->pushButtonRefreshCustomerTable, &QPushButton::clicked, this, &Rentacar::on_pushButtonRefreshCustomerTable_clicked);
    // connect(ui->pushButtonBack, &QPushButton::clicked, this, &Rentacar::on_pushButtonBack_clicked);


    if (connect_sqlite_database()) {
        load_sqlite_database(ui->tableViewSearchCar, "car");
        load_sqlite_database(ui->tableViewSearchCustomer, "customer");
        load_sqlite_database(ui->tableViewBooking, "rent");
    } else {
        QMessageBox::information(this, "Error", "Database cannot be opened!");
    }
}

Rentacar::~Rentacar()
{
    delete ui;
}

bool Rentacar::connect_sqlite_database() {
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

void Rentacar::load_sqlite_database(QTableView *tableView, const QString &tableName, const QString &querySearch) {
    QSqlQuery query;

    if (querySearch.isEmpty()) {
        query.exec(QString("SELECT * FROM %1").arg(tableName));
    } else {
        query.exec(querySearch);
    }

    QStandardItemModel *reflection_model = new QStandardItemModel();
    reflection_model->setColumnCount(query.record().count());

    while (query.next()) {
        QList<QStandardItem *> row_data;
        for (int column{0}; column < query.record().count(); ++column) {
            QStandardItem *item = new QStandardItem(query.value(column).toString());
            row_data.append(item);
        }
        reflection_model->appendRow(row_data);
    }

    tableView->setModel(reflection_model);
    tableView->verticalHeader()->setVisible(false);

    QStringList columnLabels;
    if (tableName == "car") {
        columnLabels = {"Id", "Regist. Number", "Brand", "Model", "ModYear", "Gear", "Fuel Type", "Price", "Status"};
    } else if (tableName == "customer") {
        columnLabels = {"Customer Id", "Name", "TelNum", "Email", "Adress"};
    } else if (tableName == "rent") {
        columnLabels = {"Rent Id", "Customer Id", "Car Id", "Start Date", "End Date"};
    }

    for (int column{0}; column < qMin(query.record().count(), columnLabels.size()); ++column) {
        reflection_model->setHeaderData(column, Qt::Horizontal, columnLabels[column]);
    }
}

void Rentacar::on_pushButtonAssigningCarToCustomer_clicked() {
    if(db.open()) {
        QString customer_id = ui->lineEditAssignCustomerId_2->text();
        QString car_id = ui->lineEditAssignCarId_2->text();
        QDate start_date = ui->dateEditRentStarts_2->date();
        QDate end_date = ui->dateEditRentEnds_2->date();

        QSqlQuery check_query;
        check_query.prepare("SELECT * FROM rent WHERE carId = :car_id AND ((startDate BETWEEN :start_date AND :end_date) OR (endDate BETWEEN :start_date AND :end_date) OR (startDate < :start_date AND endDate > :end_date))");
        check_query.bindValue(":car_id", car_id);
        check_query.bindValue(":start_date", start_date.toString("yyyy-MM-dd"));
        check_query.bindValue(":end_date", end_date.toString("yyyy-MM-dd"));

        if (check_query.exec() && check_query.next()) {
            ui->labelResultBooking->setText("This car is already rented in the selected date.");
            return;
        }

        QSqlQuery query;
        query.prepare("INSERT INTO rent (rentId, customerId, carId, startDate, endDate) VALUES (null, :customer_id, :car_id, :start_date, :end_date)");
        query.bindValue(":customer_id", customer_id);
        query.bindValue(":car_id", car_id);
        query.bindValue(":start_date", start_date.toString("yyyy-MM-dd"));
        query.bindValue(":end_date", end_date.toString("yyyy-MM-dd"));

        if(query.exec()) {
            ui->labelResultBooking->setText("Assignment is performed successfully!");
        } else {
            ui->labelResultBooking->setText("Assignment cannot be completed");
            qDebug() << "Error: " << query.lastError().text();
        }
    } else {
        ui->labelResultBooking->setText("Database is not connected");
    }

    load_sqlite_database(ui->tableViewBooking, "rent");
}

void Rentacar::on_pushButtonUnassigningAgreement_clicked() {
    if(db.open()) {
        QString id = ui->lineEditRentId_2->text();
        QSqlQuery query;

        if(query.exec("DELETE FROM rent WHERE rentId = " + id)) {
            ui->labelResultBooking->setText("Assignment is unassigned successfully");
        } else {
            ui->labelResultBooking->setText("Unassignment cannot be completed");
        }
    } else {
        ui->labelResultBooking->setText("Database is not connected");
    }

    load_sqlite_database(ui->tableViewBooking, "rent");
}

void Rentacar::on_pushButtonSearchCar_clicked() {
    QString registration_number = ui->lineEditSearchRegistrationNumber_2->text();
    QString querySearch = "SELECT * FROM car WHERE registration_number = '" + registration_number + "'";

    load_sqlite_database(ui->tableViewSearchCar, "car", querySearch);
}

void Rentacar::on_pushButtonSearchCustomer_clicked() {
    QString customer_name = ui->lineEditSearchCustomerName_2->text();
    QString querySearch = "SELECT * FROM customer WHERE name = '" + customer_name + "'";

    load_sqlite_database(ui->tableViewSearchCustomer, "customer", querySearch);
}

void Rentacar::on_pushButtonRefreshCarTable_clicked() {
    load_sqlite_database(ui->tableViewSearchCar, "car");
}

void Rentacar::on_pushButtonRefreshCustomerTable_clicked() {
    load_sqlite_database(ui->tableViewSearchCustomer, "customer");
}


void Rentacar::on_pushButtonBack_clicked() {
    qDebug() << "Back button clicked";
    MainWindow *mainWindowWidget = new MainWindow();
    mainWindowWidget->show();
    this->close();  // Close the current Car widget
}
