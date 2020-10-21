#ifndef MAINLOGIC_H
#define MAINLOGIC_H

#include <QObject>

class MainLogic : public QObject
{
    Q_OBJECT
public:
    explicit MainLogic(QObject *parent = nullptr);

signals:

};

#endif // MAINLOGIC_H
