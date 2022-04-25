#ifndef CONVEYERSTATEMACHINE_H
#define CONVEYERSTATEMACHINE_H

#include <QObject>
#include <QStateMachine>
#include <QAbstractTransition>
#include <QEvent>
#include <QDebug>
#include <memory>
#include "conveyer.h"
#include "globals.h"
#include <iostream>
#include <tuple>


/*
 *
 * Реализация конечного автомата применительно к работе конвейера
 *
 */

class ConveyerStateMachine : public QObject
{
    Q_OBJECT
    std::unique_ptr<QStateMachine> stateMachine;
    std::unique_ptr<Conveyer> conveyer;
    int cycles_counter;
    bool conveyer_is_active;
public:
    explicit ConveyerStateMachine(QObject *parent = nullptr);
    ~ConveyerStateMachine();
    void createStateMachine();
    void startWork();
    void resumeWorkAfterFault();
    void resetConveyer();
    void terminateWork();
    void addActuatingMechanism(SYS::enum_actuating_mechanism_state state) { conveyer->addActuatingMechanism(state); }
    int getMaximumNumberOfActuatingMechanisms(){ return conveyer->getMaximumNumberOfActuatingMechanisms(); }

signals:
    void startNewCycle();
    void updateConveyerState(std::tuple<int,SYS::enum_status>);

};

/*
 * вспомогательные сущности для переходов машины состояний
 * см. документация "The State Machine Framework", раздел "Events, Transitions and Guards"
*/

struct CommandEvent : public QEvent
{
    CommandEvent(const QString &val, QVariantList a = {})
    : QEvent(QEvent::Type(QEvent::User+1)),
      value(val), args(a) {
    }

    const QString value;
    const QVariantList args;
};

class CommandTransition : public QAbstractTransition
{
public:
    CommandTransition(const QString &value):
        QAbstractTransition(),
        m_value(value) {}

protected:
    virtual bool eventTest(QEvent *e) override
    {
        if (e->type() != QEvent::Type(QEvent::User+1)) // CommandEvent
        {
            return false;
        }
        CommandEvent *se = static_cast<CommandEvent*>(e);
        return (m_value == se->value);
    }

    virtual void onTransition(QEvent *ev) override {
        CommandEvent* e = dynamic_cast<CommandEvent*>(ev);
        qDebug() << "Transition trigered: " << e->value;
    }

private:
    QString m_value;
};

#endif // CONVEYERSTATEMACHINE_H
