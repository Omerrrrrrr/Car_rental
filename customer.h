#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTableView>
#include <QJsonArray>

namespace Ui {
class Customer;
}

class MainWindow;

class Customer : public QWidget
{
    Q_OBJECT

public:
    explicit Customer(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~Customer();

    void load_sqlite_database(QTableView *tableView, const QString &tableName);


private slots:
    void on_pushButtonAddCustomer_clicked();
    void on_pushButtonEditCustomer_clicked();
    void on_pushButtonRemoveCustomer_clicked();
    void on_pushButtonBack_clicked();


private:
    Ui::Customer *ui;
    QSqlDatabase db;

    Customer *customerWidget;   // kontrol et
    MainWindow *mainWindow;

    bool connect_sqlite_database();

    QJsonArray collect_customer();
};

#endif // CUSTOMER_H
