#include "widget.h"
#include "mainlogic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    MainLogic mainLogic;

    //QApplication::connect(&w, &Widget::find_way_signal, &mainLogic, &MainLogic::find_way);
    //QApplication::connect(&w, &Widget::find_all_airports_signal, &mainLogic, &MainLogic::find_all_airport);
    //QApplication::connect(&mainLogic, &MainLogic::return_way_signal, &w, &Widget::return_way);
    //QApplication::connect(&mainLogic, &MainLogic::return_all_airport_signal, &w, &Widget::return_all_airports);

    w.show();
    return a.exec();
}
