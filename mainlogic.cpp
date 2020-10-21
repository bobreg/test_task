#include "mainlogic.h"

MainLogic::MainLogic(QObject *parent) : QObject(parent)
{

}

void find_all_airport(QVector <QVector <QString>> * list_airports,
                      double current_airport,
                      double circle){
    Q_UNUSED(circle);
    Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    qDebug() << "Ищем аэропорты";
}

void find_way(QVector <QVector <QString>> * list_airports,
              double current_airport,
              double destination_airport,
              double circle){
    Q_UNUSED(circle);
    Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    Q_UNUSED(destination_airport);
    qDebug() << "Ищем путь";
}
