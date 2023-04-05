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
        expander(MM_EPREFIX, NFOVTermotable, Termotable) \
        expander(MM_EPREFIX, WFOVTermotable, Termotable) \
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
typedef float Termotable[TERMOTABLE_ROW_COUNT][TERMOTABLE_COLUMN_COUNT];
/**
    \ingroup mm_enums
    \brief Тестовое перечисление 1
*/
typedef enum {DECLARE_FRUITS(ENUM_EXPANDER_DECL)} Fruits;

/**
    \ingroup mm_enums
    \brief Тестовое перечисление 2
*/
typedef enum {DECLARE_COMMANDS(ENUM_EXPANDER_DECL_DESIGNATED)} Commands;

/**
    \ingroup mm_enums
    \brief Номера полей в \ref MMInfoStruct
*/
enum {DECLARE_MM_INFO(STRUCT_EXPANDER_AS_ENUM) MMCNT_ALL};
/**
    \ingroup mm_structs
    \brief Настройки модуля MM
*/
typedef struct {DECLARE_MM_INFO(STRUCT_EXPANDER_AS_FIELD)} __attribute__((packed)) MMInfoStruct;


#define REQUEST_EPREFIX RQ
#define DECLARE_REQUEST(expander) \
        expander(REQUEST_EPREFIX, fruit, Fruits, 8) \
        expander(REQUEST_EPREFIX, cmd, Commands, 8)

enum {DECLARE_REQUEST(BITFIELDS_EXPANDER_AS_ENUM)};
typedef struct {DECLARE_REQUEST(BITFIELDS_EXPANDER_AS_FIELD)} __attribute__((packed)) Request;

// принтеры MMInfo
int sprint_mm(char *stream, const MMInfoStruct *s);
int sprint_Termotable(char *stream, const Termotable table);
int sprint_Fruits(char *stream, Fruits fruit);
int sprint_Commands(char *stream, Commands cmd);
// строкофикаторы перечислений
const char *stringify_fruit(Fruits fruit);
const char *stringify_cmd(Commands cmd);

int sprint_request(char *stream, const Request *s);

#endif // MMINFO_H
