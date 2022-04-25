#ifndef ACTUATINGMECHANISM_H
#define ACTUATINGMECHANISM_H


#include <QObject>
#include <chrono>
#include "globals.h"

/*
 *
 * Реализация класса исполнительного механизма
 *
 */

class ActuatingMechanism : public QObject
{
    Q_OBJECT
    SYS::enum_actuating_mechanism_state state;
public:
    explicit ActuatingMechanism(SYS::enum_actuating_mechanism_state st = SYS::enum_actuating_mechanism_state::NORMAL, QObject *parent = nullptr);
    void setState(SYS::enum_actuating_mechanism_state st) { state = st; }
    SYS::enum_actuating_mechanism_state getState(){ return state; }
    int doWork(); // Задержка 1 c. + возврат 1 => задача выполнена
    int doWorkWithFault(); // Задержка 1 с. + возврат 0 => аварийная ситуация
signals:

};

#endif // ACTUATINGMECHANISM_H
