/**
    \file utils.h
    \brief Служебный файл
*/
#ifndef UTILS_H
#define UTILS_H

#include <inttypes.h>

#define CONCAT(x, y) x##y
#define CONCAT_(x, y) x##_##y

#define DO_PRAGMA(x) _Pragma(#x)
#if (defined(__GNUC__) && ((__GNUC__ > 4) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))) || defined(__clang__)
#define IGNORE_WARNING_PUSH(warning) \
    DO_PRAGMA(GCC diagnostic push) \
    DO_PRAGMA(GCC diagnostic ignored warning)
#define IGNORE_WARNING_POP() \
    DO_PRAGMA(GCC diagnostic pop)
#else
#define IGNORE_WARNING_PUSH(warning)
#define IGNORE_WARNING_POP()
#endif

// -------------------- struct utils --------------------
#define STRUCT_EXPANDER_AS_FIELD(_eprefix, member, type) type member;
#define STRUCT_EXPANDER_AS_ENUM(eprefix, member, _type) CONCAT_(eprefix, member),
#define STRUCT_EXPANDER_AS_PRINTER(_eprefix, member, type) \
    end_stream += sprintf(end_stream, #member ": "); \
    IGNORE_WARNING_PUSH("-Waddress-of-packed-member") \
    end_stream += CONCAT_(sprint, type)(end_stream, s->member); \
    IGNORE_WARNING_POP() \
    end_stream += sprintf(end_stream, "\n");
/**
    \brief Получение порядкового номера поля структуры
*/
#define STRUCT_FIELD_NUMBER(eprefix, member) CONCAT_(eprefix, member)
/**
    \brief Получение кода поля структуры, что является битовым флагом
*/
#define STRUCT_FIELD_CODE(eprefix, member) (1 << STRUCT_FIELD_NUMBER(eprefix, member))
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
    int CONCAT_(sprint, type) (char *stream, const type *s) { \
        char *end_stream = stream; \
        declarator(STRUCT_EXPANDER_AS_PRINTER, eprefix) \
        return end_stream - stream; \
    }
// -------------------- bitfields utils --------------------
#define BITFIELDS_EXPANDER_AS_FIELD(_eprefix, member, type, size) type member: size;
#define BITFIELDS_EXPANDER_AS_ENUM(eprefix, member, _type, _size) CONCAT_(eprefix, member),
#define BITFIELDS_EXPANDER_AS_PRINTER(_eprefix, member, type, _size) \
    end_stream += sprintf(end_stream, #member ": "); \
    end_stream += sprint##_##type(end_stream, s->member); \
    end_stream += sprintf(end_stream, "\n");
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
    int CONCAT_(sprint, type) (char *stream, const type *s) { \
        char *end_stream = stream; \
        declarator(BITFIELDS_EXPANDER_AS_PRINTER, eprefix) \
        return end_stream - stream; \
    }
// -------------------- enum utils --------------------
#define ENUM_EXPANDER_DECL(eprefix, name) CONCAT_(eprefix, name),
#define ENUM_EXPANDER_STR(_eprefix, name) #name,
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
    static const char *const CONCAT_(type, str)[] = {declarator(ENUM_EXPANDER_STR, eprefix)}; \
    const char *CONCAT_(stringify, type)(type e) {return CONCAT_(type, str)[e];} \
    int CONCAT_(sprint, type)(char *stream, type e) {return sprintf(stream, "%s", CONCAT_(type, str)[e]);}

#define ENUM_EXPANDER_DECL_DESIGNATED(eprefix, name, val) CONCAT_(eprefix, name) = val,
#define ENUM_EXPANDER_STR_DESIGNATED(_eprefix, name, val) [val] = #name,
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
    static const char *const CONCAT_(type, str)[] = {declarator(ENUM_EXPANDER_STR_DESIGNATED, eprefix)}; \
    const char *CONCAT_(stringify, type)(type e) {return CONCAT_(type, str)[e];} \
    int CONCAT_(sprint, type)(char *stream, type e) {return sprintf(stream, "%s", CONCAT_(type, str)[e]);}

// -------------------- regular printers --------------------
#define sprint_char(buf, val) sprintf(buf, "%c", val)
#define sprint_int(buf, val) sprintf(buf, "%d", val)
#define sprint_signed(buf, val) sprintf(buf, "%d", val)
#define sprint_unsigned(buf, val) sprintf(buf, "%u", val)
#define sprint_size_t(buf, val) sprintf(buf, "%zu", val)
#define sprint_ssize_t(buf, val) sprintf(buf, "%zd", val)
#define sprint_time_t(buf, val) sprintf(buf, "%lld", (long long)val)
#define sprint_ptrdiff_t(buf, val) sprintf(buf, "%0*tX", (int)(2 * sizeof(void*)), val)
#define sprint_bool(buf, val) sprintf(buf, "%s", val ? "true" : " false")
#define sprint__Bool(buf, val) sprint_bool(buf, val)
#define sprint_uint8_t(buf, val) sprintf(buf, "%" PRIu8, val)
#define sprint_uint16_t(buf, val) sprintf(buf, "%" PRIu16, val)
#define sprint_uint32_t(buf, val) sprintf(buf, "%" PRIu32, val)
#define sprint_uint64_t(buf, val) sprintf(buf, "%" PRId64, val)
#define sprint_int8_t(buf, val) sprintf(buf, "%" PRId8, val)
#define sprint_int16_t(buf, val) sprintf(buf, "%" PRId16, val)
#define sprint_int32_t(buf, val) sprintf(buf, "%" PRId32, val)
#define sprint_int64_t(buf, val) sprintf(buf, "%" PRId64, val)
#define sprint_float(buf, val) sprintf(buf, "%g", val)
#define sprint_double(buf, val) sprintf(buf, "%lg", val)

#endif // UTILS_H
