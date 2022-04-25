#ifndef CONVEYER_H
#define CONVEYER_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include <QFuture>
#include <memory>
#include <vector>
#include "actuatingmechanism.h"

/*
 *
 *  Реализация класса конвейера
 *
 */

class Conveyer : public QObject
{
    Q_OBJECT
    std::vector<std::unique_ptr<ActuatingMechanism>> actuatingMechanismsOfConveyer;
    int number_of_actuating_mechanisms; // Количество исполнительных механизмов. Выбрано значение QThread::idealThreadCount(),
                                        // т.к. все задачи должны быть в разных потоках (исполнительные механизмы работают параллельно)
    std::vector<QFuture<int>> taskResults;
public:
    explicit Conveyer(QObject *parent = nullptr);
    void addActuatingMechanism(SYS::enum_actuating_mechanism_state state);
    void createDefaultConveyer();
    void resetConveyer();
    std::vector<QFuture<int>> runConveyer();
    int getMaximumNumberOfActuatingMechanisms(){ return QThread::idealThreadCount(); }
    void setMaximumNumberOfActuatingMechanisms(int max_numb){ number_of_actuating_mechanisms = max_numb; } // значение, большее чем QThread::idealThreadCount() приведет к
                                                                                                           // неполному распараллеливанию задач между исполнительными механизмами


signals:


};

#endif // CONVEYER_H
