#include "conveyerstatemachine.h"
#include <QApplication>

ConveyerStateMachine::ConveyerStateMachine(QObject *parent) :   QObject(parent),
                                                                stateMachine(std::make_unique<QStateMachine>(this)),
                                                                conveyer(std::make_unique<Conveyer>()),
                                                                cycles_counter(0),
                                                                conveyer_is_active(true)
{
    createStateMachine();
}

ConveyerStateMachine::~ConveyerStateMachine()
{
    conveyer->resetConveyer();
    stateMachine->stop();
}

void ConveyerStateMachine::createStateMachine()
{
    QState *initialState = new QState(stateMachine.get());
    QState *workingState = new QState();// выполнение операций каждого исполнительного механизма
    QState *faultState = new QState(); // состояние отказа, в которое происходит переход после соответствующего сигнала от исполнительного механизма
                                       // (сигнал аварии или сигнал обрыва связи)
    QState *finalState = new QState(); // конечное состояние, в которое происходит переход после успешного завершения операций всех исполнительных механизмов
    QState *resetState = new QState(); // состояение приостановки

    stateMachine->addState(workingState);
    stateMachine->addState(faultState);
    stateMachine->addState(finalState);
    stateMachine->addState(resetState);

    CommandTransition *transitionToWorkingState = new CommandTransition(QStringLiteral("jumpToWorkingState"));
    transitionToWorkingState->setTargetState(workingState);
    // в рабочее состояние возможен переход из начального состояния, конечного состояния или после отказа
    initialState->addTransition(transitionToWorkingState);
    faultState->addTransition(transitionToWorkingState);

    CommandTransition *transitionToFaultState = new CommandTransition(QStringLiteral("jumpToFaultState"));
    transitionToFaultState->setTargetState(faultState);
    workingState->addTransition(transitionToFaultState);

    CommandTransition *transitionToFinalState = new CommandTransition(QStringLiteral("jumpToFinalState"));
    transitionToFinalState->setTargetState(finalState);
    // завершиться можно только из рабочего состояния
    workingState->addTransition(transitionToFinalState);

    CommandTransition *transitionToInitialState = new CommandTransition(QStringLiteral("jumpToInitialState"));
    transitionToInitialState->setTargetState(initialState);
    finalState->addTransition(transitionToInitialState);
    resetState->addTransition(transitionToInitialState);

    CommandTransition *transitionToResetState = new CommandTransition(QStringLiteral("jumpToResetState"));
    transitionToResetState->setTargetState(resetState);
    finalState->addTransition(transitionToResetState);

    // Из начального состояния производится переход в рабочее после
    // сигнала о начале первого цикла
    initialState->addTransition(this,SIGNAL(startNewCycle()),workingState);

    connect(initialState,&QState::entered,[&]()
    {
        stateMachine->postEvent(new CommandEvent("jumpToWorkingState"));
    });

    connect(workingState,&QState::entered,[&]()
    {
        std::cout << "In working state..." << std::endl;
        std::cout << "Counter of working cycles is: " << cycles_counter << std::endl;
        emit updateConveyerState(std::make_tuple(cycles_counter,SYS::enum_status::RUNNING));
        auto returnResult = conveyer->runConveyer();
        // если хотя бы одно значение 0 ( состояние отказа ), то переходим в соответствующее состояние
        if((std::any_of(returnResult.begin(),returnResult.end(),[&](int value){ return value == 0; })))
        {
            stateMachine->postEvent(new CommandEvent("jumpToFaultState"));
        }
        // в противном случае продвигаем конвейерную ленту на 1
        else
        {
            stateMachine->postEvent(new CommandEvent("jumpToFinalState"));
        }

    });

    connect(faultState,&QState::entered,[&]()
    {
        std::cout << "Entering fault state... Do something to eliminate the problem..." << std::endl;
        emit updateConveyerState(std::make_tuple(cycles_counter,SYS::enum_status::FAULT));
    });

    connect(resetState,&QState::entered,[&]()
    {
        std::cout << "In reset state" << std::endl;
        cycles_counter = 0;
        conveyer->resetConveyer();
        emit updateConveyerState(std::make_tuple(cycles_counter,SYS::enum_status::RESET));
    });

    connect(finalState,&QState::entered,[&]()
    {
        qDebug() << "In final state...";
        emit updateConveyerState(std::make_tuple(cycles_counter,SYS::enum_status::DONE));
        cycles_counter++;
        if(!conveyer_is_active)
        {
            std::cout << "jump to reset state" << std::endl;
            stateMachine->postEvent(new CommandEvent("jumpToResetState"));
        }
        else
        {
            std::cout << "jump to initial state" << std::endl;
            stateMachine->postEvent(new CommandEvent("jumpToInitialState"));
        }
    });

    stateMachine->setInitialState(initialState);
    stateMachine->start();
}


void ConveyerStateMachine::startWork()
{
    if(!conveyer_is_active)
    {
        conveyer_is_active = true;
        stateMachine->postEvent(new CommandEvent("jumpToInitialState"));
    }
    else
    {
        emit startNewCycle();
    }
}

void ConveyerStateMachine::resumeWorkAfterFault()
{
    //сбрасываем все ошибочные задачи
    conveyer->resetConveyer();
    //создаем конвейер, в котором ошибок уже нет
    conveyer->createDefaultConveyer();
    stateMachine->postEvent(new CommandEvent("jumpToWorkingState"));
}

void ConveyerStateMachine::resetConveyer()
{
    conveyer_is_active = false;
}

void ConveyerStateMachine::terminateWork()
{
    stateMachine->stop();
    std::cout << "Exit the application...";
    QApplication::instance()->quit();
}
