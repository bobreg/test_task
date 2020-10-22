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
    double lat;
    double lon;
    QVector <double> XY;
    QVector <index_with_airport> list_air;
    //------создадим базу из ху координат всех аэропортов (пригодится для поиска пути)-----
    if(XY_for_all_airports.isEmpty()){
        for(int i = 0; i < count_airports; i++){
            lat = (&list_airports)[i][1].toDouble();
            lon = (&list_airports)[i][2].toDouble();
            XY = dec_coord(lat, lon);
            XY_for_all_airports.append(XY);
            //qDebug() << XY << (&list_airports)[i];
        }
        //qDebug() << XY_for_all_airports.size();
    }
    X_current_airport = XY_for_all_airports[current_airport][0];
    Y_current_airport = XY_for_all_airports[current_airport][1];
    //qDebug() << "Ищем аэропорты" << (&list_airports)[current_airport] << circle;
    list_air = compare_cycle_airports(list_airports, count_airports, current_airport, circle);
    emit return_all_airport_signal(list_air);
}

void MainLogic::find_way(const QVector <QString> & list_airports,
                         int count_airports,
                         int current_airport,
                         int destination_airport,                        // индекс аэропорта назначения в базе
                         double circle){

    QVector <QVector <QString>> way_list_air;
    double lat;
    double lon;
    QVector <double> XY;
    QVector <QVector <QString>> list_air;
    //------создадим базу из ху координат всех аэропортов (пригодится для поиска пути)-----
    if(XY_for_all_airports.isEmpty()){
        for(int i = 0; i < count_airports; i++){
            lat = (&list_airports)[i][1].toDouble();
            lon = (&list_airports)[i][2].toDouble();
            XY = dec_coord(lat, lon);
            XY_for_all_airports.append(XY);
            //qDebug() << XY << (&list_airports)[i];
        }
        //qDebug() << XY_for_all_airports.size();
    }
    X_current_airport = XY_for_all_airports[current_airport][0];
    Y_current_airport = XY_for_all_airports[current_airport][1];
    X_destination_airport = XY_for_all_airports[destination_airport][0];
    Y_destination_airport = XY_for_all_airports[destination_airport][1];

    // Расстояние между текущим аэропортом полёта и аэропортом назначения (нужна для определения тех точек которые удаляют нас от назначения)
    // Их не используем для поиска пути в ходе работы алгоритма поиска
    distanse_A_B = hypot(X_current_airport - X_destination_airport, Y_current_airport - Y_destination_airport);


    //qDebug() << "Ищем путь" << (&list_airports)[destination_airport] << count_airports;
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

QVector <index_with_airport> MainLogic::compare_cycle_airports(const QVector <QString> & list_airports, int count_airports, int current_index, double circle){
    QVector <index_with_airport> list_air;
    index_with_airport temp;
//------для каждого аэропорта посчитаем расстояние и сравним расст. с окружностью-----
    for(int i = 0; i < count_airports; i++){
        double distance = hypot(XY_for_all_airports[current_index][0] - XY_for_all_airports[i][0], XY_for_all_airports[current_index][1] - XY_for_all_airports[i][1]); // 0 - коорд. х, 1 - коорд. у
        if(distance < (circle * 1000)){
            //qDebug() << distance / 1000 << (&list_airports)[i][0];
            temp.index = i;
            temp.airport = (&list_airports)[i];
            list_air.append(temp);
        }
    }
    return list_air;
}
int MainLogic::next_airport(const QVector <QString> & list_airports, int count_airports, int iter, double circle){
    QVector <index_with_airport> list_air;
    double min_distance = distanse_A_B;
    int min_index;
    for(int i = 0; i < count_airports; i++){  // пробежимся по всем аэропортам и найдем те которые не далее радиуса полёта
        list_air = compare_cycle_airports(list_airports, count_airports, iter, circle);
        // для каждого такого аэропорта вычислим новую дальность до аэропорта назначения и сравним его с начальной
        // до запуска этой функции (и найдём самую минимальную)
        for(auto i : list_air){
            double x = i.airport[0].toDouble();
            double y = i.airport[1].toDouble();
            double dist = hypot(x - X_destination_airport, y - Y_destination_airport);
            if(dist < distanse_A_B){ // если новая дальность меньше исходной, то
                if(dist < min_distance){  // сравним новую дистанцию с новой минимальной,
                    //если она меньше, чем прошлая, то минимальную дистанцию нашли
                    min_distance = dist;
                    min_index = i.index;
                }
            }
        }
    }
    return min_index;
}
