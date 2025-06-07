#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include "QtWidgets/qtablewidget.h"
#include "car.h"
#include "customer.h"
#include "rentacar.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QSqlDatabase db;
    QTableWidget *QTableWidget;

private slots:
    void on_pushButtonCars_clicked();
    void on_pushButtonCustomers_clicked();
    void on_pushButtonRentacar_clicked();
    void on_pushButtonExport_clicked();
    void on_pushButtonImport_clicked();

private:
    Ui::MainWindow *ui;
    Car *carWidget;
    Customer *customerWidget;
    Rentacar *rentacarWidget;
    QMediaPlayer *player;
    QVideoWidget *videoWidget;

    QJsonArray collect_car();
    QJsonArray collect_customer();
    void insert_car_json(const QJsonArray &car_array);
    void insert_customer_json(const QJsonArray &customer_array);
    void export_json_file(const QString &json_file_name);
    void import_json_file(const QString &json_file_name);
    void loadTableWithJsondata(const QJsonArray &car_array, const QJsonArray &customer_array);
};

#endif // MAINWINDOW_H
