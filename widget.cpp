#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    int count_row_file = 0;
    //--------Настройка таблицы аэродромов----------------
    ui->tableWidget->setColumnCount(3);
    QList <QString> strl = { "Аэродром", "Широта", "Долгота" };
    ui->tableWidget->setHorizontalHeaderLabels(QStringList(strl));  // установка заголовка таблицы
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  // установка только одиночного выбора в таблице
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // установка выбора только строки

    //ui->tableWidget->setItem(0, 0, new QTableWidgetItem ("asd"));


    //--------Считывание файла----------------------------

    QFile f("airports.csv");
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);
    while (!stream.atEnd()) {
        count_row_file++;
        QString str = stream.readLine();
        //qDebug() << str;
        QVector <QString> t_vector;
        for(QString item : str.split("\t")){
            t_vector.append(item);
        }
        vector_airports.append(t_vector);
    }
    //----------Заполнение таблицы---------------

    ui->tableWidget->setRowCount(count_row_file-1);
    for(int i = 1; i < count_row_file; i++){
        for (int j = 0; j < 3; j++) {
            ui->tableWidget->setItem(i-1, j, new QTableWidgetItem (vector_airports[i][j]));
        }
    }
    vector_airports.remove(0);
}

Widget::~Widget()
{
    delete ui;
}

//-----------Выбрать текущий аэропорт---------------
void Widget::on_pushButton_clicked()
{
    number_current = ui->tableWidget->currentRow() + 1;
    current_airport = "";
    current_airport.append(vector_airports[number_current][0]);
    current_airport.append(" / ");
    current_airport.append(vector_airports[number_current][1]);
    current_airport.append(" / ");
    current_airport.append(vector_airports[number_current][2]);
    ui->label_4->setText(current_airport);
}
//-----------Выбрать аэропорт назначения------------
void Widget::on_pushButton_2_clicked()
{
    numder_destination = ui->tableWidget->currentRow() + 1;
    destination_airport = "";
    destination_airport.append(vector_airports[numder_destination][0]);
    destination_airport.append(" / ");
    destination_airport.append(vector_airports[numder_destination][1]);
    destination_airport.append(" / ");
    destination_airport.append(vector_airports[numder_destination][2]);
    ui->label_6->setText(destination_airport);
}
//-----------Найти аэропорты не далее N км----------
void Widget::on_pushButton_3_clicked()
{
    //QString temp = ui->
}
//-----------Найти кратчайший маршрут---------------
void Widget::on_pushButton_4_clicked()
{

}
