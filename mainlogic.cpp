#include "mainlogic.h"

MainLogic::MainLogic(QObject *parent) : QObject(parent)
{
    a_wgs = 6378137;
    alpha_wgs = 1/298.257223563;
    e_pow_two = 2 * alpha_wgs - pow(alpha_wgs, 2);
}

void MainLogic::find_all_airport(const QVector <QString> & list_airports,  // константная ссылка на список аэропортов (точнее на первый аэропорт в базе)
                                 int count_air,                       // количество аэропортов в базе (нужно для обхода базы и невыхода за диапазон)
                                 int current_airport,                      // индекс текущего аэропорта в базе
                                 double circle){                           // радиус поиска
    count_airports = count_air;
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
    list_air = compare_cycle_airports(list_airports, current_airport, circle);
    emit return_all_airport_signal(list_air);
}

void MainLogic::find_way(const QVector <QString> & list_airports,
                         int count_air,
                         int current_airport,
                         int destination_airport,                        // индекс аэропорта назначения в базе
                         double circle){
    count_airports = count_air;
    way_by_airports.clear();
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
    //-------------------------------------------------------------------------------------
    X_current_airport = XY_for_all_airports[current_airport][0];
    Y_current_airport = XY_for_all_airports[current_airport][1];
    X_destination_airport = XY_for_all_airports[destination_airport][0];
    Y_destination_airport = XY_for_all_airports[destination_airport][1];
    double max_d_for_prb = hypot(X_current_airport - X_destination_airport, Y_current_airport - Y_destination_airport);

    int way_current_index = current_airport;
    QVector <int> list_indexes_way;
    list_indexes_way.append(way_current_index);  // добавим аэропорт вылета

    // пока не прилетели
    while (way_current_index != destination_airport) {
        way_current_index = next_airport(list_airports, way_current_index, circle); //получим аэр. приближенный к аэр. назн.
        //qDebug() << list_indexes_way << list_indexes_way[list_indexes_way.size()-1];
        // сработает если не найден ниодин аэропорт ближе чем текущий, но можно поробовать поискать от прошлых посещённых
        if(way_current_index == -1 && !list_indexes_way.isEmpty()){
            //qDebug() << "переищем путь" << list_indexes_way << indexes_of_checked_airport;
            list_indexes_way.removeLast(); // удалим предыдущий посещённых аэропорта
            if(list_indexes_way.isEmpty()){ // если список посещённых аэропортов опустел, то путь не найден
                //qDebug() << "не нашли путь";
                break;
            }
            way_current_index = list_indexes_way[list_indexes_way.size()-1]; // ещё одна попытка найти путь от прошлого посещённого аэропорта
        }else if(way_current_index == destination_airport){ // путь найден
            //qDebug() << "нашли путь";
            emit change_progress_bar(100);
            list_indexes_way.append(way_current_index);// добавим аэропорт прилёта
            break;
        }
        if(way_current_index == -1 && list_indexes_way.isEmpty()){
            //qDebug() << "не нашли путь";
            emit return_way_signal(way_by_airports);
            break;
        }else{
            if(!list_indexes_way.contains(way_current_index)){ // если такого аэропорта нет в списке аэропортов пути
                list_indexes_way.append(way_current_index);
            }
        }
        emit change_progress_bar(int((max_d_for_prb - last_distanse_A_B)*100/max_d_for_prb));
    }

    for(auto i : list_indexes_way){
        way_by_airports.append((&list_airports)[i]);
    }
    emit return_way_signal(way_by_airports);
    list_indexes_way.clear();  // почистим все списки для нового поиска
    way_by_airports.clear();
    indexes_of_checked_airport.clear();
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

QVector <index_with_airport> MainLogic::compare_cycle_airports(const QVector <QString> & list_airports, int current_index, double circle){
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

int MainLogic::next_airport(const QVector <QString> & list_airports, int iter, double circle){
    //необходимые переменные
    //дистанция между аэропортом назначения и предыдущей позицией
    last_distanse_A_B = hypot(XY_for_all_airports[iter][0] - X_destination_airport, XY_for_all_airports[iter][1] - Y_destination_airport);


    QVector <index_with_airport> list_air; // список проверяемых аэропортов в радиусе от переданного аэропорта
    //qDebug() << distanse_A_B / 1000 << iter << (&list_airports)[iter];
    double dist_to_d = 0;     // текущая дистанция от исследуемого аэропорта до аэропорта назначения
    int max_index = -1;       // индекс максимально удалённого аэропорта от текущего в базе который мы вернём
                              // если такой найдём из всех найденных на текущем шаге

    // пробежимся по всем аэропортам и найдем те которые не далее радиуса полёта
    for(int i = 0; i < count_airports; i++){
        list_air = compare_cycle_airports(list_airports, iter, circle);
    }

    // для каждого такого аэропорта вычислим новую дальность до аэропорта назначения и сравним его с начальной
    for(auto i : list_air){
        if(!indexes_of_checked_airport.contains(i.index)){ //если мы ещё не посещали этот аэропорт
            double x = XY_for_all_airports[i.index][0];    // возьмём его координаты из базы декартовых координат
            double y = XY_for_all_airports[i.index][1];
            dist_to_d = hypot(x - X_destination_airport, y - Y_destination_airport); //вычислим расст. до аэр. назначения
            //dist_from_c = hypot(XY_for_all_airports[iter][0] - XY_for_all_airports[i.index][0], XY_for_all_airports[iter][1] - XY_for_all_airports[i.index][1]); //это для отладки
            if(dist_to_d == 0){  // если мы прилетели в аэропорт назначения то закончим поиск и вернём его индекс
                return i.index;
            }
            //qDebug() << dist_to_d/1000 << i.index << i.airport;
            if(dist_to_d <= last_distanse_A_B){ // если новая дальность меньше исходной, то
                indexes_of_checked_airport.append(i.index); // если нам подходит аэропорт добавим его в список посещённых
                last_distanse_A_B = dist_to_d; // переопределим дальность до аэр. назначения от этого подходящего нам аэропорта
                max_index = i.index;      // запишем индекс этого аэропорта
                // если на следующем шаге окажется что дальность dist_to_d ещё меньше distanse_A_B, то запомним её
            }
        }
    }
    //qDebug() << dist_to_d /1000 << max_index << "---------";
    last_distanse_A_B = dist_to_d;
    return max_index;
}
