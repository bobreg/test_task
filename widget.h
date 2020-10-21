#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDebug>


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
    QVector <QVector <QString>> vector_airports;  // база всех считанных аэропортов из файла
    QString current_airport;
    QString destination_airport;

    int number_current;
    int numder_destination;
    double max_range_fly;
    double max_circle;
};
#endif // WIDGET_H
