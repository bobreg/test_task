#include "mainlogic.h"

MainLogic::MainLogic(QObject *parent) : QObject(parent)
{
    a_wgs = 6378137;
    alpha_wgs = 1/298.257223563;
    e_pow_two = 2 * alpha_wgs - pow(alpha_wgs, 2);
}

void MainLogic::find_all_airport(const QVector <QString> & list_airports,  // константная ссылка на список аэропортов (точнее на первый аэропорт в базе)
                                 int count_airports,                       // количество аэропортов в базе (нужно для обхода базы и невыхода за диапазон)
                                 int current_airport,                      // индекс текущего аэропорта в базе
                                 double circle){                           // радиус поиска

    QVector <QVector <QString>> list_air;
    double lat;
    double lon;
    QVector <double> XY;
    double lat_current = (&list_airports)[current_airport][1].toDouble();  // расчёт координат текущего аэропорта
    double lon_current = (&list_airports)[current_airport][2].toDouble();
    QVector <double> XY_current_airport = dec_coord(lat_current, lon_current);
    //qDebug() << "Ищем аэропорты" << (&list_airports)[current_airport] << circle;
    for(int i = 0; i <= count_airports; i++){ // для каждого аэропорта посчитаем координаты и сравним расст. с текущим
        lat = (&list_airports)[i][1].toDouble();
        lon = (&list_airports)[i][2].toDouble();
        XY = dec_coord(lat, lon);
        double distance = hypot(XY_current_airport[0] - XY[0], XY_current_airport[1] - XY[1]); // 0 - коорд. х, 1 - коорд. у
        //qDebug() << distance / 1000 << (&list_airports)[i][0];
        if(distance < (circle * 1000)){
            list_air.append((&list_airports)[i]);
        }
    }
    emit return_all_airport_signal(list_air);
}

void MainLogic::find_way(const QVector <QString> & list_airports,
                         int count_airports,
                         int current_airport,
                         int destination_airport,                        // индекс аэропорта назначения в базе
                         double circle){
    Q_UNUSED(circle);
    //Q_UNUSED(list_airports);
    Q_UNUSED(current_airport);
    //Q_UNUSED(destination_airport);
    qDebug() << "Ищем путь" << (&list_airports)[destination_airport] << count_airports;
    emit return_way_signal((&list_airports)[count_airports]);
}


QVector <double> MainLogic::dec_coord(double lat, double lon){
    lat = (lat * M_PI) / 180;  // перевод градусов радианы. (sin и cos считают в радианах)
    lon = (lon * M_PI) / 180;
    double N = a_wgs / (sqrt(1 - (e_pow_two * pow(sin(lat), 2))));  // радиус кривизны первого вертикала
    //qDebug() << N << lat;
    double X = N * cos(lat) * cos(lon);
    //qDebug() << X;
    double Y = N * cos(lat) * sin(lon);
    //qDebug() << Y;
    QVector <double> XY;
    XY.append(X);
    XY.append(Y);
    return XY;
}
