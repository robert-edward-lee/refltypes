#ifndef UTILS_H
#define UTILS_H

#define CONCAT(x, y) x##y
#define CONCAT_(x, y) x##_##y

// -------------------- struct utils --------------------
/**
   Применение:
    ```c
    #define DECLARE_<STRUCT>(expander) \
            expander(<STRUCT_PREFIX>, <field0>, <type_field0>) \
            expander(<STRUCT_PREFIX>, <field1>, <type_field1>) \
            ...

    enum {DECLARE<STRUCT>(STRUCT_EXPANDER_AS_ENUM) <STRUCT_PREFIX>_ALL};
    typedef struct {DECLARE<STRUCT>(STRUCT_EXPANDER_AS_FIELD)} <StructType>;
    ```
    \note <STRUCT_PREFIX> это префикс для перечислений, с помощью которых кодируются поля структуры
*/

/**
    \brief Расширитель объявления структуры для объявления её полей
*/
#define STRUCT_EXPANDER_AS_FIELD(_eprefix, member, type) type member;
/**
    \brief Расширитель объявления структуры для объявления перечисления для хранения порядковых номеров её полей
*/
#define STRUCT_EXPANDER_AS_ENUM(eprefix, member, _type) CONCAT_(eprefix, member),
/**
    \brief Расширитель объявления структуры для её превращении в строку типа:
    field1: value1
    field2: value2
    ...

    Пример использования:
    ```c
    int sprint_<smthng>(char *stream, const <Struct Type> *s) {
        char *end_stream = stream;
        DECLARE_<SOME_STRUCT>(STRUCT_EXPANDER_AS_PRINTER, <PREFIX>)
        return end_stream - stream;
    }
    ```
    Где stream - выходной буфер, s - адрес структуры, временная переменная end_stream - указатель на конец сообщения.
    Сущности в фигурных скобках указываются конкретно для каждого случая, остальные же имена оставить как есть.
*/
#define STRUCT_EXPANDER_AS_PRINTER(_eprefix, member, type) \
    end_stream += sprintf(end_stream, #member ": "); \
    end_stream += sprint##_##type(end_stream, s->member); \
    end_stream += sprintf(end_stream, "\n");
/**
    \brief Получение порядкового номера поля структуры
*/
#define STRUCT_FIELD_NUMBER(eprefix, member) CONCAT_(eprefix, member)
/**
    \brief Получение кода поля структуры, что является битовым флагом
*/
#define STRUCT_FIELD_CODE(eprefix, member) (1 << STRUCT_FIELD_NUMBER(eprefix, member))
// -------------------- bitfields utils --------------------
#define BITFIELDS_EXPANDER_AS_FIELD(_eprefix, member, type, size) type member: size;
#define BITFIELDS_EXPANDER_AS_ENUM(eprefix, member, _type, _size) CONCAT_(eprefix, member),
#define BITFIELDS_EXPANDER_AS_PRINTER(_eprefix, member, type, _size) \
    end_stream += sprintf(end_stream, #member ": "); \
    end_stream += sprint##_##type(end_stream, s->member); \
    end_stream += sprintf(end_stream, "\n");
// -------------------- enum utils --------------------
/**
    \brief Расширитель объявления перечисления для его инициализации
*/
#define ENUM_EXPANDER_DECL(eprefix, name) CONCAT_(eprefix, name),
/**
    \brief Расширитель объявления перечисления в качестве массива строк
*/
#define ENUM_EXPANDER_STR(_eprefix, name) #name,
/**
    \brief Расширитель объявления перечисления для его инициализации при назначенном инициализаторе
    \note Нельзя пользоваться отрицательными значениями, возможно пока
*/
#define ENUM_EXPANDER_DECL_DESIGNATED(eprefix, name, val) CONCAT_(eprefix, name) = val,
/**
    \brief Расширитель объявления перечисления в качестве массива строк при назначенном инициализаторе
    \note Нельзя пользоваться отрицательными значениями, возможно пока
*/
#define ENUM_EXPANDER_STR_DESIGNATED(_eprefix, name, val) [val] = #name,

// -------------------- printers --------------------
#define sprint_uint8_t(buf, val) sprintf(buf, "%u", val)
#define sprint_uint16_t(buf, val) sprintf(buf, "%u", val)
#define sprint_uint32_t(buf, val) sprintf(buf, "%u", val)
#define sprint_int8_t(buf, val) sprintf(buf, "%d", val)
#define sprint_int16_t(buf, val) sprintf(buf, "%d", val)
#define sprint_int32_t(buf, val) sprintf(buf, "%d", val)
#define sprint_float(buf, val) sprintf(buf, "%g", val)
#define sprint_double(buf, val) sprintf(buf, "%lg", val)


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

#endif // UTILS_H
