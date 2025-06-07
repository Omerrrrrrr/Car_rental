#ifndef CAR_H
#define CAR_H

#include <QWidget>
#include <QTableView>
#include <QSqlDatabase>
#include <QJsonArray>

namespace Ui {
class Car;
}

class MainWindow;

class Car : public QWidget
{
    Q_OBJECT

public:
    explicit Car(QWidget *parent = nullptr, MainWindow *mainWindow = nullptr);
    ~Car();

    void load_sqlite_database(QTableView *tableView, const QString &tableName);

private slots:
    void on_pushButtonAddCar_clicked();
    void on_pushButtonEditCar_clicked();
    void on_pushButtonRemoveCar_clicked();
    void on_pushButtonBack_clicked();

private:
    Ui::Car *ui;
    QSqlDatabase db;
    Car *carWidget;
    MainWindow *mainWindow;

    bool connect_sqlite_database();
    QJsonArray collect_car();
};

#endif // CAR_H
