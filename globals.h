#ifndef GLOBALS_H
#define GLOBALS_H

#include <type_traits>
#include <QString>
#include <QMetaEnum>

#define SILENT_MODE 1

namespace SYS
{
    enum class enum_status { RUNNING, DONE, FAULT, RESET };

    enum class enum_actuating_mechanism_state { NORMAL,FAULTY };


    /* возвращает приведенное к родному типу значение класса enum'a */
    template<typename E>
    constexpr std::underlying_type_t<E>
    toUType(E enumerator) noexcept {
        return static_cast<std::underlying_type_t<E>>(enumerator);
    }

    QString enumToString(enum_status st);
}


#endif // GLOBALS_H
