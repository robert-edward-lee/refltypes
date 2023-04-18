#ifndef MMINFO_H
#define MMINFO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "refltype.h"

#define THERMO_TABLE_ROWS 2
#define THERMO_TABLE_COLUMNS 15
//  ---------------------------------------------
#define DECLARE_COMMANDS(expander, _) \
        expander(_, READ_LINE_THERMO, 0x05) \
        expander(_, DRIVE_LINE, 0x01) \
        expander(_, DRIVE_SPEED, 0x07) \
        expander(_, ERROR, 0)
TYPEDEF_ENUM_DESIGNATED(Commands, CMD, DECLARE_COMMANDS)
//  ---------------------------------------------
#define DECLARE_FRUITS(expander, _) \
        expander(_, BANANA) \
        expander(_, APPLE) \
        expander(_, PEAR) \
        expander(_, TOMATO) \
        expander(_, ROOT_CROP)
TYPEDEF_ENUM(Fruits, FR, DECLARE_FRUITS)
//  ---------------------------------------------
typedef float ThermoTable[THERMO_TABLE_ROWS][THERMO_TABLE_COLUMNS];
int sprint_ThermoTable(char *stream, const ThermoTable table, int lvl, const char *prefix);
//  ---------------------------------------------
#define DECLARE_REQUEST(expander, _) \
        expander(_, fruit, Fruits, 8) \
        expander(_, cmd, Commands, 8)
TYPEDEF_BITFIELDS(Request, RQ, DECLARE_REQUEST, packed)
#define REQUEST_FIELD_CODE(member) STRUCT_FIELD_CODE(RQ, member)
//  ---------------------------------------------
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
        expander(_, req, Request) \
        expander(_, is_cool, bool) \
        expander(_, timer, time_t) \
        expander(_, ptrdiff, ptrdiff_t) \
        expander(_, sptr, ptr_t)
TYPEDEF_STRUCT(MmTest, MMCNT, DECLARE_MM_INFO, packed)
#define MM_INFO_FIELD_CODE(member) STRUCT_FIELD_CODE(MMCNT, member)

#endif // MMINFO_H
