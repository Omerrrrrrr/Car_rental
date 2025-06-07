#ifndef RENTACAR_H
#define RENTACAR_H

#include <QWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTableView>

namespace Ui {
class Rentacar;
}

class Rentacar : public QWidget
{
    Q_OBJECT

public:
    explicit Rentacar(QWidget *parent = nullptr);
    ~Rentacar();

    void load_sqlite_database(QTableView *tableView, const QString &tableName, const QString &querySearch = "");

private slots:
    void on_pushButtonAssigningCarToCustomer_clicked();
    void on_pushButtonUnassigningAgreement_clicked();
    void on_pushButtonSearchCar_clicked();
    void on_pushButtonSearchCustomer_clicked();
    void on_pushButtonRefreshCarTable_clicked();
    void on_pushButtonRefreshCustomerTable_clicked();
    void on_pushButtonBack_clicked();

private:
    Ui::Rentacar *ui;
    QSqlDatabase db;
    Rentacar *RentacarWidget;

    bool connect_sqlite_database();

};

#endif // RENTACAR_H
