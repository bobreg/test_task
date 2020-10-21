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
    void return_all_airport_signal(QVector <QString>);
    void return_way_signal(QVector <QString>);

public slots:
    void find_all_airport(const QVector <QString> &, int, double, double);
    void find_way(const QVector <QString> &, int, double, double, double);

};

#endif // MAINLOGIC_H
