#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDebug>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;

    QFile air;
    QVector <QVector <QString>> list_airports;    // база всех считанных аэропортов из файла
    QString current_airport;                      // текущий аэропорт
    QString destination_airport;                  // аэропорт назначения

    int number_current = 0;                           // индекс текущего аэропорта в базе
    int number_destination = 0;                       // индекс аэропорта назначения в базе
    double max_circle = 10;                            // радиус максимального поиска
    double max_range_fly = 10;                         // максимальная дальность полёта самолёта

signals:
    void find_all_airports_signal(const QVector <QString> &, int, int, double);
    void find_way_signal(const QVector <QString> &, int, int, int, double);

public slots:
    void return_all_airports(QVector <QVector <QString>>);
    void return_way(QVector <QString>);

};
#endif // WIDGET_H
