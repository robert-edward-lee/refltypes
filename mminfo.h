#ifndef MMINFO_H
#define MMINFO_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "utils.h"

/**
    \ingroup mm_defs
    \brief Количество строк в таблице термокомпенсации
*/
#define THERMO_TABLE_ROWS 2
/**
    \ingroup mm_defs
    \brief Общее количество столбцов в таблице термокомпенсации
*/
#define THERMO_TABLE_COLUMNS 15

//  ---------------------------------------------
/**
    \ingroup
    \brief Тестовое перечисление 2
*/
#define DECLARE_COMMANDS(expander, _) \
        expander(_, READ_LINE_THERMO, 0x05) \
        expander(_, DRIVE_LINE, 0x01) \
        expander(_, DRIVE_SPEED, 0x07) \
        expander(_, ERROR, 0)
/**
    \ingroup mm_enums
    \brief Тестовое перечисление 2
*/
TYPEDEF_ENUM_DESIGNATED(Commands, CMD, DECLARE_COMMANDS)
//  ---------------------------------------------
/**
    \ingroup
    \brief Тестовое перечисление 1
*/
#define DECLARE_FRUITS(expander, _) \
        expander(_, BANANA) \
        expander(_, APPLE) \
        expander(_, PEAR) \
        expander(_, TOMATO) \
        expander(_, ROOT_CROP)
/**
    \ingroup mm_enums
    \brief Тестовое перечисление 1
*/
TYPEDEF_ENUM(Fruits, FR, DECLARE_FRUITS)
//  ---------------------------------------------
/**
    \ingroup mm_structs
    \brief Таблица поправок на терморасстраиваемость
*/
typedef float ThermoTable[THERMO_TABLE_ROWS][THERMO_TABLE_COLUMNS];
int sprint_ThermoTable(char *stream, const ThermoTable table);
//  ---------------------------------------------
/**
    \ingroup mm_defs
    \brief Настройки модуля MM
*/
#define DECLARE_MM_INFO(expander, _) \
        expander(_, NfovThermoTable, ThermoTable) \
        expander(_, WfovThermoTable, ThermoTable) \
        expander(_, NfovBasePosition, float) \
        expander(_, WfovBasePosition, float) \
        expander(_, CalibrationPosition, float) \
        expander(_, FocusSpeedMin, float) \
        expander(_, FocusSpeedMax, float) \
        expander(_, FOVChangeTimeMax, float) \
        expander(_, BeginChangeFocusSpeedTime, float) \
        expander(_, EndChangeFocusSpeedTime, float) \
        expander(_, StatePollingTime, uint8_t) \
        expander(_, ExchangeErrorNumber, uint8_t) \
        expander(_, TemperatureDelta, float) \
        expander(_, Fruit, Fruits) \
        expander(_, Command, Commands) \
        expander(_, is_cool, bool) \
        expander(_, timer, time_t) \
        expander(_, ptrdiff, ptrdiff_t) \
        expander(_, sptr, ptr_t)
/**
    \ingroup mm_structs
    \brief Настройки модуля MM
*/
TYPEDEF_STRUCT(MmTest, MMCNT, DECLARE_MM_INFO, packed)
/**
    \ingroup mm_defs
    \brief Код номера параметра в структуре, подготовленный для отправки в сообщении
*/
#define MM_INFO_FIELD_CODE(member) STRUCT_FIELD_CODE(MMCNT, member)
//  ---------------------------------------------
#define DECLARE_REQUEST(expander, _) \
        expander(_, fruit, Fruits, 8) \
        expander(_, cmd, Commands, 8)
TYPEDEF_BITFIELDS(Request, RQ, DECLARE_REQUEST, packed)
#define REQUEST_FIELD_CODE(member) STRUCT_FIELD_CODE(RQ, member)

#endif // MMINFO_H
