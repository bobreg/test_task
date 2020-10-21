#include "mainlogic.h"

MainLogic::MainLogic(QObject *parent) : QObject(parent)
{

}

void MainLogic::find_all_airport(const QVector <QString> & list_airports,  // константная ссылка на список аэропортов (точнее на первый аэропорт в базе)
                                 int count_airports,                       // количество аэропортов в базе (нужно для обхода базы и невыхода за диапазон)
                                 double current_airport,                   // индекс текущего аэропорта в базе
                                 double circle){                           // радиус поиска
    Q_UNUSED(circle);
    //Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    qDebug() << "Ищем аэропорты" << (&list_airports)[0] << count_airports;
    emit return_all_airport_signal((&list_airports)[0]);
}

void MainLogic::find_way(const QVector <QString> & list_airports,
                         int count_airports,
                         double current_airport,
                         double destination_airport,                        // индекс аэропорта назначения в базе
                         double circle){
    Q_UNUSED(circle);
    //Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    Q_UNUSED(destination_airport);
    qDebug() << "Ищем путь" << (&list_airports)[count_airports] << count_airports;
    emit return_way_signal((&list_airports)[1]);
}

