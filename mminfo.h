#ifndef MMINFO_H
#define MMINFO_H

#include <stdint.h>
#include "utils.h"

/**
    \ingroup mm_defs
    \brief Префикс для перечисления для хранения порядковых номеров её полей
*/
#define MM_EPREFIX MMCNT
/**
    \ingroup mm_defs
    \brief Настройки модуля MM
*/
#define DECLARE_MM_INFO(expander) \
        expander(MM_EPREFIX, NFOVTermotable, ThermoTable) \
        expander(MM_EPREFIX, WFOVTermotable, ThermoTable) \
        expander(MM_EPREFIX, NFOVBasePosition, float) \
        expander(MM_EPREFIX, WFOVBasePosition, float) \
        expander(MM_EPREFIX, CalibrationPosition, float) \
        expander(MM_EPREFIX, FocusSpeedMin, float) \
        expander(MM_EPREFIX, FocusSpeedMax, float) \
        expander(MM_EPREFIX, FOVChangeTimeMax, float) \
        expander(MM_EPREFIX, BeginChangeFocusSpeedTime, float) \
        expander(MM_EPREFIX, EndChangeFocusSpeedTime, float) \
        expander(MM_EPREFIX, StatePollingTime, uint8_t) \
        expander(MM_EPREFIX, ExchangeErrorNumber, uint8_t) \
        expander(MM_EPREFIX, TemperatureDelta, float) \
        expander(MM_EPREFIX, Fruit, Fruits) \
        expander(MM_EPREFIX, Command, Commands)

#define FRUITS_EPREFIX FR
/**
    \ingroup
    \brief Тестовое перечисление 1
*/
#define DECLARE_FRUITS(expander) \
        expander(FRUITS_EPREFIX, BANANA) \
        expander(FRUITS_EPREFIX, APPLE) \
        expander(FRUITS_EPREFIX, PEAR) \
        expander(FRUITS_EPREFIX, TOMATO) \
        expander(FRUITS_EPREFIX, ROOT_CROP) \
        expander(FRUITS_EPREFIX, ALL)

#define COMMANDS_EPREFIX CMD
/**
    \ingroup
    \brief Тестовое перечисление 2
*/
#define DECLARE_COMMANDS(expander) \
        expander(COMMANDS_EPREFIX, READ_LINE_THERMO, 0x05) \
        expander(COMMANDS_EPREFIX, DRIVE_LINE, 0x01) \
        expander(COMMANDS_EPREFIX, DRIVE_SPEED, 0x07) \
        expander(COMMANDS_EPREFIX, ERROR, 0)


/**
    \ingroup mm_defs
    \brief Количество столбцов в таблицах термокомпенсации
*/
#define TERMOTABLE_COLUMN_COUNT 15
/**
    \ingroup mm_defs
    \brief Общее количество строк в таблице термокомпенсации
*/
#define TERMOTABLE_ROW_COUNT 2

/**
    \ingroup mm_defs
    \brief Получение кол-ва полей в структуре \ref MMInfoStruct
*/
#define MM_INFO_FIELDS() MMCNT_ALL
/**
    \ingroup mm_defs
    \brief Код номера параметра в структуре, подготовленный для отправки в сообщении
*/
#define MM_FIELD_CODE(member) STRUCT_FIELD_CODE(MM_EPREFIX, member)

/**
    \ingroup mm_structs
    \brief Таблица поправок на терморасстраиваемость
*/
typedef float ThermoTable[TERMOTABLE_ROW_COUNT][TERMOTABLE_COLUMN_COUNT];
/**
    \ingroup mm_enums
    \brief Тестовое перечисление 1
*/
TYPEDEF_ENUM(Fruits, FRUITS_EPREFIX, DECLARE_FRUITS)
/**
    \ingroup mm_enums
    \brief Тестовое перечисление 2
*/
TYPEDEF_ENUM_DESIGNATED(Commands, COMMANDS_EPREFIX, DECLARE_COMMANDS)
/**
    \ingroup mm_structs
    \brief Настройки модуля MM
*/
TYPEDEF_STRUCT(MmTest, MM_EPREFIX, DECLARE_MM_INFO, packed)


#define REQUEST_EPREFIX RQ
#define DECLARE_REQUEST(expander) \
        expander(REQUEST_EPREFIX, fruit, Fruits, 8) \
        expander(REQUEST_EPREFIX, cmd, Commands, 8)
TYPEDEF_BITFIELDS(Request, REQUEST_EPREFIX, DECLARE_REQUEST)

int sprint_ThermoTable(char *stream, const ThermoTable table);

#endif // MMINFO_H
