#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <QObject>
#include <QDebug>
#include <QStack>
#include <math.h>
#include "mystruct.h"



class MainLogic : public QObject
{
    Q_OBJECT
public:
    explicit MainLogic(QObject *parent = nullptr);

private:
    QVector <QVector <QString>> way_by_airports;
    QVector <QVector <double>> XY_for_all_airports;

    QVector <int> indexes_of_checked_airport; // сюда кладём индексы всех аэропортов в которые залетали и их больше проверять не надо

    double X_current_airport = 0;  // координаты аэропортов в декартовой системе
    double Y_current_airport = 0;
    double X_destination_airport = 0;
    double Y_destination_airport = 0;
    int count_airports;
    double last_distanse_A_B;

    QVector <double> dec_coord(double, double);  //перевод гео координат в декартовые по гост 51794-2008
    QVector <index_with_airport> compare_cycle_airports(const QVector <QString> &, int, double);  //возвращает список аэропортов радиусе от текущего
    // функция возвращает нам максимально удалённый аэропорт от текущей позиции и максимально
    // приближенный к аэропорту назначения в радиусе перелёта
    int next_airport(const QVector <QString> &, int, double);

    // переменные для пересчёта координат
    double a_wgs;             // общеземной эллипсоид с большой полуосью
    double alpha_wgs;         // сжатие общеземного эллипсоида
    double e_pow_two;         // эксцентриситет эллипсоида

signals:
    void return_all_airport_signal(QVector <index_with_airport>);
    void return_way_signal(QVector <QVector <QString>>);
    void change_progress_bar(int);

public slots:
    void find_all_airport(const QVector <QString> &, int, int, double);
    void find_way(const QVector <QString> &, int, int, int, double);

};

#endif // MAINLOGIC_H
