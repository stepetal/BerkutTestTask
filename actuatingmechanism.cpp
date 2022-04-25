#include "actuatingmechanism.h"
#include <QEventLoop>
#include <thread>

ActuatingMechanism::ActuatingMechanism(SYS::enum_actuating_mechanism_state st, QObject *parent) : QObject(parent),state(st)
{

}

int ActuatingMechanism::doWork()
{
    QEventLoop elp;
    elp.processEvents();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 1;
}

int ActuatingMechanism::doWorkWithFault()
{
    QEventLoop elp;
    elp.processEvents();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}


