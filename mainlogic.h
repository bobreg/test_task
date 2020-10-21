#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <QObject>
#include <QDebug>

class MainLogic : public QObject
{
    Q_OBJECT
public:
    explicit MainLogic(QObject *parent = nullptr);

private:
    QVector <QVector <QString>> way_by_airports;
    QVector <QVector <QString>> all_airports;

signals:
    void return_all_airport_signal(QVector <QVector <QString>> *);
    void return_way_signal(QVector <QVector <QString>> *);

public slots:
    void find_all_airport(QVector <QVector <QString>> *, double, double);
    void find_way(QVector <QVector <QString>> *, double, double, double);

};

#endif // MAINLOGIC_H
