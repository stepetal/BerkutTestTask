#include "globals.h"



QString SYS::enumToString(enum_status st)
{
    QString message;
    switch (st)
    {
    case enum_status::DONE:
    {
        message = QString("Очередной цикл успешно завершен");
        break;
    }
    case enum_status::RUNNING:
    {
        message = QString("Запущен новый цикл");
        break;
    }

    case enum_status::FAULT:
    {
        message = QString("Отказ исполнительного механизма. Нужно вмешательство оператора");
        break;
    }

    case enum_status::RESET:
    {
        message = QString("Сброс текущего состояния конвейера");
        break;
    }
    default:
        message = QString();
        break;
    }
    return message;
}
