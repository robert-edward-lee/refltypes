#ifndef REFLTYPES_H
#define REFLTYPES_H

#include <inttypes.h>

#define DO_PRAGMA(x) _Pragma(#x)
#if (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))) || defined(__clang__)
#define IGNORE_WARNING_PUSH(warning) DO_PRAGMA(GCC diagnostic push) DO_PRAGMA(GCC diagnostic ignored warning)
#define IGNORE_WARNING_POP() DO_PRAGMA(GCC diagnostic pop)
#else
#define IGNORE_WARNING_PUSH(warning)
#define IGNORE_WARNING_POP()
#endif

#ifndef _cplusplus
#define static_assert(condition, reason) \
        IGNORE_WARNING_PUSH("-Wpedantic") ({ \
            extern int __attribute__((error("static assertion failed: " reason))) _(); \
            condition ? 0 : _(); \
        }); IGNORE_WARNING_POP()
#endif

#define _CONCAT(x, y) x##y
#define CONCAT(x, y) _CONCAT(x, y)
#define _CONCAT_(x, y) x##_##y
#define CONCAT_(x, y) _CONCAT_(x, y)

// -------------------- struct utils --------------------
#define STRUCT_EXPANDER_AS_FIELD(_eprefix, member, type) type member;
#define STRUCT_EXPANDER_AS_ENUM(eprefix, member, _type) CONCAT_(eprefix, member),
#define STRUCT_EXPANDER_AS_PRINTER(_eprefix, member, type) \
    end_stream += CONCAT_(sprint, type)(end_stream, s.member, lvl+4, #member);
/**
    \brief Получение порядкового номера поля структуры
*/
#define STRUCT_FIELD_NUMBER(eprefix, member) STRUCT_EXPANDER_AS_ENUM(eprefix, member, _)
/**
    \brief Получение кода поля структуры, что является битовым флагом
*/
#define STRUCT_FIELD_CODE(eprefix, member) ((unsigned long long)1 << STRUCT_FIELD_NUMBER(eprefix, member))
/**
    \param type Тип структуры
    \param eprefix Префикс перед элементами перечисления, необходим для рефлексии структуры
    \param declarator Декларация полей структуры, которое само является макроопределением
    \code
    #define DECLARE_<STRUCT>(expander, _) \
            expander(_, <field0>, <type0>) \
            expander(_, <field1>, <type1>) \
            expander(_, <field2>, <type2>) \
            ...
    \endcode
    \param ... Опциональные параметры для атрибутов в GNU стиле, например packed, aligned(n)

    \brief Определение типа структуры, которое автоматически генерирует функцию sprint_<type>, которая работает
    аналогично sprintf для данной структуры
    \note Для работы рефлексии необходимо чтобы для всех типов полей существовала функция или макроопределение типа
    sprint_<type>
*/
#define TYPEDEF_STRUCT(type, eprefix, declarator, ...) \
    typedef struct CONCAT_(_, type) {declarator(STRUCT_EXPANDER_AS_FIELD, eprefix)} \
    __attribute__((__VA_ARGS__)) type; \
    enum {declarator(STRUCT_EXPANDER_AS_ENUM, eprefix) CONCAT_(eprefix, ALL)}; \
    int CONCAT_(sprint, type)(char *stream, type s, int lvl, const char *prefix) { \
        char *end_stream = stream; \
        if(prefix) end_stream += sprintf(end_stream, "%s: ", prefix); \
        end_stream += sprintf(end_stream, "\n"); \
        IGNORE_WARNING_PUSH("-Waddress-of-packed-member") \
        declarator(STRUCT_EXPANDER_AS_PRINTER, eprefix) \
        IGNORE_WARNING_POP() \
        return end_stream - stream; \
    }
// -------------------- bitfields utils --------------------
#define BITFIELDS_EXPANDER_AS_FIELD(_eprefix, member, type, size) type member: size;
#define BITFIELDS_EXPANDER_AS_ENUM(eprefix, member, _type, _size) STRUCT_EXPANDER_AS_ENUM(eprefix, member, _type)
#define BITFIELDS_EXPANDER_AS_PRINTER(_eprefix, member, type, _size) \
    end_stream += CONCAT_(sprint, type)(end_stream, s.member, lvl+4, #member);
/**
    \param type Тип структуры
    \param eprefix Префикс перед элементами перечисления, необходим для рефлексии структуры
    \param declarator Декларация полей структуры, которое само является макроопределением
    \code
    #define DECLARE_<STRUCT>(expander, _) \
            expander(_, <field0>, <type0>, <size0>) \
            expander(_, <field1>, <type1>, <size1>) \
            expander(_, <field2>, <type2>, <size2>) \
            ...
    \endcode
    \param ... Опциональные параметры для атрибутов в GNU стиле, например packed, aligned(n)

    \brief Определение типа структуры с битовыми полями, которое автоматически генерирует функцию sprint_<type>,
    которая работает аналогично sprintf для данной структуры
    \note Для работы рефлексии необходимо чтобы для всех типов битовых полей существовала функция или макроопределение
    типа sprint_<type>
*/
#define TYPEDEF_BITFIELDS(type, eprefix, declarator, ...) \
    typedef struct CONCAT_(_, type) {declarator(BITFIELDS_EXPANDER_AS_FIELD, eprefix)} \
    __attribute__((__VA_ARGS__)) type; \
    enum {declarator(BITFIELDS_EXPANDER_AS_ENUM, eprefix) CONCAT_(eprefix, ALL)}; \
    int CONCAT_(sprint, type)(char *stream, type s, int lvl, const char *prefix) { \
        char *end_stream = stream; \
        if(prefix) end_stream += sprintf(end_stream, "%*s%s: ", lvl, "", prefix); \
        end_stream += sprintf(end_stream, "\n"); \
        IGNORE_WARNING_PUSH("-Waddress-of-packed-member") \
        declarator(BITFIELDS_EXPANDER_AS_PRINTER, eprefix) \
        IGNORE_WARNING_POP() \
        return end_stream - stream; \
    }
// -------------------- enum utils --------------------
#define ENUM_EXPANDER_DECL(eprefix, name) CONCAT_(eprefix, name),
#define ENUM_EXPANDER_PRINTER(eprefix, name) case CONCAT_(eprefix, name): return #name;
/**
    \param type Тип перечисления
    \param eprefix Префикс перед элементом перечисления
    \param declarator Декларация элементов перечисления, которое само является макроопределением
    \code
    #define DECLARE_<ENUM>(expander, _) \
            expander(_, <element0>) \
            expander(_, <element1>) \
            expander(_, <element2>) \
            ...
    \endcode

    \brief Определение типа перечисления, которое автоматически генерирует функции sprint_<type>, которая работает
    аналогично sprintf и stringify_<type> которая возвращает строку с именем элемента перечисления
*/
#define TYPEDEF_ENUM(type, eprefix, declarator) \
    typedef enum {declarator(ENUM_EXPANDER_DECL, eprefix) CONCAT_(eprefix, ALL)} type; \
    const char *CONCAT_(stringify, type)(type e) {switch(e) \
    {declarator(ENUM_EXPANDER_PRINTER, eprefix) default: return "unknown";}} \
    int CONCAT_(sprint, type)(char *stream, type e, int lvl, const char *prefix) \
    {return sprintf(stream, "%*s%s: %s\n", lvl, "", prefix, CONCAT_(stringify, type)(e));}

#define ENUM_EXPANDER_DECL_DESIGNATED(eprefix, name, val) CONCAT_(eprefix, name) = val,
#define ENUM_EXPANDER_PRINTER_DESIGNATED(eprefix, name, _val) case CONCAT_(eprefix, name): return #name;
/**
    \param type Тип перечисления
    \param eprefix Префикс перед элементом перечисления
    \param declarator Декларация элементов перечисления, которое само является макроопределением
    \code
    #define DECLARE_<ENUM>(expander, _) \
            expander(_, <element0>, <value0>) \
            expander(_, <element1>, <value1>) \
            expander(_, <element2>, <value2>) \
            ...
    \endcode

    \brief Определение типа перечисления с обозначенным инициализатором, которое автоматически генерирует функции
    sprint_<type>, которая работает аналогично sprintf и stringify_<type> которая возвращает строку с именем элемента
    перечисления
    \note Нельзя пользоваться отрицательными значениями и с осторожностью подходить к значениям элементов перечисления,
    так как для рефлексии формируется массив указателей на строки и его размер будет равен величине самого большого
    элемента плюс один
*/
#define TYPEDEF_ENUM_DESIGNATED(type, eprefix, declarator) \
    typedef enum {declarator(ENUM_EXPANDER_DECL_DESIGNATED, eprefix)} type; \
    const char *CONCAT_(stringify, type)(type e) {switch(e) \
    {declarator(ENUM_EXPANDER_PRINTER_DESIGNATED, eprefix) default: return "unknown";}} \
    int CONCAT_(sprint, type)(char *stream, type e, int lvl, const char *prefix) \
    {return sprintf(stream, "%*s%s: %s\n", lvl, "", prefix, CONCAT_(stringify, type)(e));}

// void* alias
typedef void *ptr_t;
// -------------------- regular printers --------------------
#define sprint_char(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %c\n", lvl, "", prefix, val)
#define sprint_int(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %d\n", lvl, "", prefix, val)
#define sprint_signed(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %d\n", lvl, "", prefix, val)
#define sprint_unsigned(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %u\n", lvl, "", prefix, val)
#define sprint_size_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %zu\n", lvl, "", prefix, val)
#define sprint_ssize_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %zd\n", lvl, "", prefix, val)
#define sprint_time_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %lld\n", lvl, "", prefix, (long long)val)
#define sprint_ptrdiff_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %0*tX\n", lvl, "", prefix, (int)(2 * sizeof(ptr_t)), val)
#define sprint_ptr_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %0*tX\n", lvl, "", prefix, (int)(2 * sizeof(ptr_t)), (uintptr_t)val)
#define sprint_bool(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %s\n", lvl, "", prefix, val ? "true" : " false")
#define sprint__Bool(stream, val, lvl, prefix) sprint_bool(stream, val, lvl, prefix)
#define sprint_uint8_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRIu8 "\n", lvl, "", prefix, val)
#define sprint_uint16_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRIu16 "\n", lvl, "", prefix, val)
#define sprint_uint32_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRIu32 "\n", lvl, "", prefix, val)
#define sprint_uint64_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRIu64 "\n", lvl, "", prefix, val)
#define sprint_int8_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRId8 "\n", lvl, "", prefix, val)
#define sprint_int16_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRId16 "\n", lvl, "", prefix, val)
#define sprint_int32_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRId32 "\n", lvl, "", prefix, val)
#define sprint_int64_t(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %" PRId64 "\n", lvl, "", prefix, val)
#define sprint_float(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %g\n", lvl, "", prefix, val)
#define sprint_double(stream, val, lvl, prefix) sprintf(stream, "%*s%s: %lg\n", lvl, "", prefix, val)

#endif // REFLTYPES_H
