#include "mainlogic.h"

MainLogic::MainLogic(QObject *parent) : QObject(parent)
{

}

void MainLogic::find_all_airport(const QVector <QVector <QString>> & list_airports,
                      double current_airport,
                      double circle){
    Q_UNUSED(circle);
    //Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    qDebug() << "Ищем аэропорты";

    emit return_all_airport_signal((&list_airports)[0]);
}

void MainLogic::find_way(const QVector <QString> & list_airports,
              double current_airport,
              double destination_airport,
              double circle){
    Q_UNUSED(circle);
    //Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    Q_UNUSED(destination_airport);
    qDebug() << "Ищем путь";
    qDebug() << list_airports[0];
    emit return_way_signal(list_airports);
}

