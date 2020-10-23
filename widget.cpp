#include "widget.h"
#include "ui_widget.h"



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    int count_row_file = 0;
    //--------Настройка таблиц аэродромов----------------
// главная таблица
    ui->tableWidget->setColumnCount(3);
    QList <QString> strl = { "Аэродром", "Широта", "Долгота" };
    ui->tableWidget->setHorizontalHeaderLabels(QStringList(strl));  // установка заголовка таблицы
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);  // установка только одиночного выбора в таблице
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  // установка выбора только строки

// таблица аэропортов не далее N км
    ui->tableWidget_2->setColumnCount(3);
    ui->tableWidget_2->setHorizontalHeaderLabels(QStringList(strl));
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

// таблица аэропортов по маршруту
    ui->tableWidget_3->setColumnCount(3);
    ui->tableWidget_3->setHorizontalHeaderLabels(QStringList(strl));
    ui->tableWidget_3->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);
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
        list_airports.append(t_vector);
    }
    //----------Заполнение таблицы---------------

    ui->tableWidget->setRowCount(count_row_file-1);
    for(int i = 1; i < count_row_file; i++){
        for (int j = 0; j < 3; j++) {
            ui->tableWidget->setItem(i-1, j, new QTableWidgetItem (list_airports[i][j]));
        }
    }
    list_airports.remove(0);
    //qDebug() << list_airports.size();
}

Widget::~Widget()
{
    delete ui;
}

//-----------Выбрать текущий аэропорт---------------
void Widget::on_pushButton_clicked()
{
    //Если в таблице ничего не выбрано, то она возвращает "-1". Ниже "-1" меняем на "0"
    number_current = (ui->tableWidget->currentRow() < 0) ? 0 : ui->tableWidget->currentRow();
    current_airport = "";
    current_airport.append(list_airports[number_current][0]);
    current_airport.append(" / ");
    current_airport.append(list_airports[number_current][1]);
    current_airport.append(" / ");
    current_airport.append(list_airports[number_current][2]);
    ui->label_4->setText(current_airport);
}
//-----------Выбрать аэропорт назначения------------
void Widget::on_pushButton_2_clicked()
{
    //Если в таблице ничего не выбрано, то она возвращает "-1". Ниже "-1" меняем на "0"
    number_destination = (ui->tableWidget->currentRow() < 0) ? 0 : ui->tableWidget->currentRow();
    destination_airport = "";
    destination_airport.append(list_airports[number_destination][0]);
    destination_airport.append(" / ");
    destination_airport.append(list_airports[number_destination][1]);
    destination_airport.append(" / ");
    destination_airport.append(list_airports[number_destination][2]);
    ui->label_6->setText(destination_airport);
}
//-----------Найти аэропорты не далее N км----------
void Widget::on_pushButton_3_clicked()
{
    QString temp = ui->lineEdit->text();
    bool flagToDouble;
    max_circle = temp.toDouble(&flagToDouble);
    if(flagToDouble == false){
        QMessageBox msg(QMessageBox::Critical, "Ошибка", "Пожалуйста введите \n целое или десятичное число!", QMessageBox::Ok);
        msg.exec();
    }else if(max_circle <= 0){
        QMessageBox msg(QMessageBox::Critical, "Ошибка", "Пожалуйста введите \n положительное число!", QMessageBox::Ok);
        msg.exec();
    }else if(ui->label_4->text() == "Nan / 0.0 / 0.0"){
        QMessageBox msg(QMessageBox::Critical, "Ошибка", "Не выбран текущий аэропорт!", QMessageBox::Ok);
        msg.exec();
    }else{
        emit find_all_airports_signal(list_airports[0], list_airports.size(), number_current, max_circle);
    }
}
//-----------Найти кратчайший маршрут---------------
void Widget::on_pushButton_4_clicked()
{
    QString temp = ui->lineEdit_2->text();
    bool flagToDouble;
    max_range_fly = temp.toDouble(&flagToDouble);
    if(flagToDouble == false){
        QMessageBox msg(QMessageBox::Critical, "Ошибка", "Пожалуйста введите \n целое или десятичное число!", QMessageBox::Ok);
        msg.exec();
    }else if(max_range_fly <= 0){
        QMessageBox msg(QMessageBox::Critical, "Ошибка", "Пожалуйста введите \n положительное число!", QMessageBox::Ok);
        msg.exec();
    }else if(ui->label_4->text() == "Nan / 0.0 / 0.0" || ui->label_6->text() == "Nan / 0.0 / 0.0"){
        QMessageBox msg(QMessageBox::Critical, "Ошибка", "Не выбран текущий аэропорт\nи аэропорт назначения!", QMessageBox::Ok);
        msg.exec();
    }else{
        emit find_way_signal(list_airports[0], list_airports.size(), number_current, number_destination, max_range_fly);
    }
}


void Widget::return_all_airports(QVector <index_with_airport> list_all_airports){
    ui->tableWidget_2->setRowCount(list_all_airports.size());
    for(int i = 0; i < list_all_airports.size(); i++){
        for (int j = 0; j < 3; j++) {
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem (list_all_airports[i].airport[j]));
        }
    }
    /*Q_UNUSED(list_all_airports);
    qDebug() << "Поискали аэропорты";
    for(auto i : list_all_airports){
        qDebug() << i;
    }*/
}

void Widget::return_way(QVector <QString> list_way_by_airports){
    //Q_UNUSED(list_way_by_airports);
    qDebug() << "Поискали путь" << list_way_by_airports;
}

