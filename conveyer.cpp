#include "conveyer.h"
#include <QEventLoop>


Conveyer::Conveyer(QObject *parent) :   QObject(parent),
                                        number_of_actuating_mechanisms(QThread::idealThreadCount())
{

}

void Conveyer::addActuatingMechanism(SYS::enum_actuating_mechanism_state state)
{
    if(actuatingMechanismsOfConveyer.size() < number_of_actuating_mechanisms)
    {
        actuatingMechanismsOfConveyer.push_back(std::make_unique<ActuatingMechanism>(state));
    }
}

void Conveyer::createDefaultConveyer()
{
    // Создаем исполнительные механизмы и задаем для них задачи
    for(auto i = 0; i < number_of_actuating_mechanisms; ++i)
    {
        actuatingMechanismsOfConveyer.push_back(std::make_unique<ActuatingMechanism>());
    }
}

void Conveyer::resetConveyer()
{
    actuatingMechanismsOfConveyer.clear();
    taskResults.clear();
}

std::vector<QFuture<int>> Conveyer::runConveyer()
{
    if(!actuatingMechanismsOfConveyer.empty())
    {
        for(auto& am : actuatingMechanismsOfConveyer)
        {
            if(am->getState() == SYS::enum_actuating_mechanism_state::NORMAL)
            {
                taskResults.push_back(QtConcurrent::run([&](){ return am->doWork(); }));
            }
            if(am->getState() == SYS::enum_actuating_mechanism_state::FAULTY)
            {
                taskResults.push_back(QtConcurrent::run([&](){ return am->doWorkWithFault(); }));
            }
        }
        //wait until all tasks finished
        bool finished{false};
        while(!finished)
        {
            QEventLoop elp;
            elp.processEvents();
            if(std::all_of(taskResults.begin(),taskResults.end(),[](QFuture<int> fut){ return fut.isFinished(); }))
            {
                finished = true;
            }
        }
        return taskResults; // RVO => возврат по значению нормален
    }
    return std::vector<QFuture<int>>();
}
