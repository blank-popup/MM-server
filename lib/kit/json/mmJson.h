#ifndef ___MM_KIT_JSON_H___
#define ___MM_KIT_JSON_H___

#include <stdbool.h>
#include <stddef.h>
// #include "../../base/mmMemory.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t)&(((TYPE*)0)->MEMBER))
#endif
#ifndef container_of
#define container_of(PTR, TYPE, MEMBER) ((TYPE*)((char*)(PTR) - offsetof(TYPE, MEMBER)))
#endif

typedef enum _JSON_type {
    json_type_error = -1,
    json_type_object = 1,
    json_type_array = 2,
    json_type_string = 3,
    json_type_number = 4,
    json_type_number_string = 5,
    json_type_boolean = 6,
    json_type_null = 7
} JSON_type;

#define JSON_SIZE_BUFFER_NUMBER (64)
#define JSON_FORMAT_NUMBER "%1.17g"

#define JSON_PHRASE_TRUE "true"
#define JSON_PHRASE_FALSE "false"
#define JSON_PHRASE_NULL "null"

#ifndef ___DEFINED__MSTRING___
#define ___DEFINED__MSTRING___
    typedef struct _mstring {
        char* value;
        size_t length;
    } mstring;
#endif

    typedef struct _JSON_name JSON_name;
    typedef struct _JSON_object JSON_object;
    typedef struct _JSON_array JSON_array;
    typedef struct _JSON_object_item JSON_object_item;
    typedef struct _JSON_array_item JSON_array_item;
    typedef struct _JSON_value JSON_value;


#define JSON_PARSE_OBJECT_OPEN (0)
#define JSON_PARSE_OBJECT_CLOSE (1)
#define JSON_PARSE_OBJECT_NAME (2)
#define JSON_PARSE_OBJECT_COLON (3)
#define JSON_PARSE_OBJECT_VALUE (4)
#define JSON_PARSE_OBJECT_COMMA (5)

#define JSON_PARSE_ARRAY_OPEN (0)
#define JSON_PARSE_ARRAY_CLOSE (1)
#define JSON_PARSE_ARRAY_VALUE (2)
#define JSON_PARSE_ARRAY_COMMA (3)

#define JSON_PARSE_STRING_OPEN (0)
#define JSON_PARSE_STRING_CLOSE (1)
#define JSON_PARSE_STRING_EXCEPT_SPECIAL (2)
#define JSON_PARSE_STRING_CONTROL_CHARACTER (3)
#define JSON_PARSE_STRING_UTF (4)

#define JSON_PARSE_NUMBER_OPEN (0)
#define JSON_PARSE_NUMBER_CLOSE (1)
#define JSON_PARSE_NUMBER_INTEGER_MINUS (2)
#define JSON_PARSE_NUMBER_INTEGER_PLUS (3)
#define JSON_PARSE_NUMBER_INTEGER_ZERO (4)
#define JSON_PARSE_NUMBER_INTEGER_NON_ZERO (5)
#define JSON_PARSE_NUMBER_INTEGER_DIGIT (6)
#define JSON_PARSE_NUMBER_FRACTION_DOT (7)
#define JSON_PARSE_NUMBER_FRACTION_DIGIT (8)
#define JSON_PARSE_NUMBER_EXPONENT_E (9)
#define JSON_PARSE_NUMBER_EXPONENT_SIGN (10)
#define JSON_PARSE_NUMBER_EXPONENT_DIGIT (11)

#define JSON_DEFAULT_NAME NULL
#define JSON_DEFAULT_JNAME NULL

#define JSON_DEFAULT_MSTRING (mstring){ NULL, 0 }
#define JSON_DEFAULT_VALUE NULL
#define JSON_VALUE_NULL NULL
#define JSON_DEFAULT_OBJECT NULL
#define JSON_OBJECT_NULL NULL
#define JSON_DEFAULT_OBJECT_ITEM NULL
#define JSON_OBJECT_ITEM_NULL NULL
#define JSON_DEFAULT_ARRAY NULL
#define JSON_ARRAY_NULL NULL
#define JSON_DEFAULT_ARRAY_ITEM NULL
#define JSON_ARRAY_ITEM_NULL NULL
#define JSON_DEFAULT_STRING NULL
#define JSON_STRING_NULL NULL
#define JSON_DEFAULT_NUMBER (0)
#define JSON_NUMBER_NULL (0)
#define JSON_DEFAULT_NUMBER_STRING NULL
#define JSON_NUMBER_STRING_NULL NULL
#define JSON_DEFAULT_BOOLEAN false
#define JSON_BOOLEAN_NULL false

#define JSON_DEFAULT_TYPE json_type_error
#define JSON_DEFAULT_LENGTH (0)
#define JSON_DEFAULT_DEPTH (0)
#define JSON_DEFAULT_ITH (1)

    void* json_malloc(size_t _size);
#define json_free(m) do { free(m); m = NULL; } while(0);
    char* json_mstrdup(const char* _content, size_t _length);

#ifndef memloc
#define memloc(s) json_malloc(s)
#endif
#ifndef memfree
#define memfree(m) json_free(m)
#endif

    JSON_name* json_name_create_mstring(mstring _mstring);
    JSON_name* json_name_create_string(const char* _string);
    JSON_name* json_name_create_number(double _number);
    JSON_name* json_name_create_number_mstring(mstring _mnumber);
    JSON_name* json_name_create_number_string(const char* _number);
    JSON_name* json_name_create_boolean(bool _boolean);
    JSON_name* json_name_create_null(void);
    void json_name_free(JSON_name* _name);

    bool json_name_is_same(JSON_name* _name0, JSON_name* _name1);
    JSON_name* json_name_duplicate(JSON_name* _name);

    JSON_name* json_object_item_get_jname(JSON_object_item* _item);
    const char* json_object_item_get_name(JSON_object_item* _item);
    JSON_object_item* json_object_get_ith_object_item(JSON_object* _object, size_t _ith);
    JSON_object_item* json_object_set_jname_value(JSON_object* _object, JSON_name* _name, JSON_value* _value);
    JSON_object_item* json_object_set_ith_jname_value(JSON_object* _object, size_t _ith, JSON_name* _name, JSON_value* _value);
    JSON_name* json_object_get_jname(JSON_object* _object, size_t _ith);
    bool json_object_has_jname(JSON_object* _object, JSON_name* _name);
    JSON_object_item* json_object_get_jname_object_item(JSON_object* _object, JSON_name* _name);

    JSON_value* json_value_create_object(void);
    JSON_value* json_value_create_array(void);
    JSON_value* json_value_create_mstring(mstring _mstring);
    JSON_value* json_value_create_string(const char* _string);
    JSON_value* json_value_create_number(double _number);
    JSON_value* json_value_create_number_mstring(mstring _mnumber);
    JSON_value* json_value_create_number_string(const char* _number);
    JSON_value* json_value_create_boolean(bool _boolean);
    JSON_value* json_value_create_null(void);
    void json_value_free(JSON_value* _value);

    JSON_value* json_value_duplicate(JSON_value* _value);
    size_t json_value_get_depth(JSON_value* _value);

    JSON_type json_value_get_type(JSON_value* _value);
    JSON_value* json_value_get_parent(JSON_value* _value);
    JSON_object* json_value_get_object(JSON_value* _value);
    JSON_array* json_value_get_array(JSON_value* _value);
    const char* json_value_get_string(JSON_value* _value);
    double json_value_get_number(JSON_value* _value);
    const char* json_value_get_number_string(JSON_value* _value);
    double json_value_get_real_number(JSON_value* _value);
    bool json_value_get_boolean(JSON_value* _value);
    bool json_value_is_null(JSON_value* _value);

    JSON_value* json_object_item_get_value(JSON_object_item* _item);
    JSON_object_item* json_object_item_get_next(JSON_object_item* _item);
    JSON_object_item* json_object_item_get_previous(JSON_object_item* _item);

    JSON_object_item* json_object_set_value(JSON_object* _object, const char* _name, JSON_value* _value);
    JSON_object_item* json_object_set_object(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_set_array(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_set_mstring(JSON_object* _object, const char* _name, mstring _mstring);
    JSON_object_item* json_object_set_string(JSON_object* _object, const char* _name, const char* _string);
    JSON_object_item* json_object_set_number(JSON_object* _object, const char* _name, double _number);
    JSON_object_item* json_object_set_number_mstring(JSON_object* _object, const char* _name, mstring _mnumber);
    JSON_object_item* json_object_set_number_string(JSON_object* _object, const char* _name, const char* _number);
    JSON_object_item* json_object_set_boolean(JSON_object* _object, const char* _name, bool _boolean);
    JSON_object_item* json_object_set_null(JSON_object* _object, const char* _name);

    JSON_object_item* json_object_set_ith_value(JSON_object* _object, size_t _ith, const char* _name, JSON_value* _value);
    JSON_object_item* json_object_set_ith_object(JSON_object* _object, size_t _ith, const char* _name);
    JSON_object_item* json_object_set_ith_array(JSON_object* _object, size_t _ith, const char* _name);
    JSON_object_item* json_object_set_ith_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mstring);
    JSON_object_item* json_object_set_ith_string(JSON_object* _object, size_t _ith, const char* _name, const char* _string);
    JSON_object_item* json_object_set_ith_number(JSON_object* _object, size_t _ith, const char* _name, double _number);
    JSON_object_item* json_object_set_ith_number_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mnumber);
    JSON_object_item* json_object_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _name, const char* _number);
    JSON_object_item* json_object_set_ith_boolean(JSON_object* _object, size_t _ith, const char* _name, bool _boolean);
    JSON_object_item* json_object_set_ith_null(JSON_object* _object, size_t _ith, const char* _name);

    size_t json_object_get_length(JSON_object* _object);
    const char* json_object_get_name(JSON_object* _object, size_t _ith);
    bool json_object_has_name(JSON_object* _object, const char* _name);
    JSON_value* json_object_get_value_wrapper(JSON_object* _object);
    JSON_object_item* json_object_get_item_head(JSON_object* _object);
    JSON_object_item* json_object_get_item_tail(JSON_object* _object);

    JSON_object_item* json_object_get_object_item(JSON_object* _object, const char* _name);
    JSON_value* json_object_get_value(JSON_object* _object, const char* _name);
    JSON_type json_object_get_type(JSON_object* _object, const char* _name);
    JSON_object* json_object_get_object(JSON_object* _object, const char* _name);
    JSON_array* json_object_get_array(JSON_object* _object, const char* _name);
    const char* json_object_get_string(JSON_object* _object, const char* _name);
    double json_object_get_number(JSON_object* _object, const char* _name);
    const char* json_object_get_number_string(JSON_object* _object, const char* _name);
    double json_object_get_real_number(JSON_object* _object, const char* _name);
    bool json_object_get_boolean(JSON_object* _object, const char* _name);
    bool json_object_is_null(JSON_object* _object, const char* _name);

    JSON_object_item* json_object_get_ith_object_item(JSON_object* _object, size_t _ith);
    JSON_value* json_object_get_ith_value(JSON_object* _object, size_t _ith);
    JSON_type json_object_get_ith_type(JSON_object* _object, size_t _ith);
    JSON_object* json_object_get_ith_object(JSON_object* _object, size_t _ith);
    JSON_array* json_object_get_ith_array(JSON_object* _object, size_t _ith);
    const char* json_object_get_ith_string(JSON_object* _object, size_t _ith);
    double json_object_get_ith_number(JSON_object* _object, size_t _ith);
    const char* json_object_get_ith_number_string(JSON_object* _object, size_t _ith);
    double json_object_get_ith_real_number(JSON_object* _object, size_t _ith);
    bool json_object_get_ith_boolean(JSON_object* _object, size_t _ith);
    bool json_object_is_ith_null(JSON_object* _object, size_t _ith);

    JSON_object* json_object_remove(JSON_object* _object, const char* _name);
    JSON_object* json_object_remove_all(JSON_object* _object);

    JSON_value* json_array_item_get_value(JSON_array_item* _item);
    JSON_array_item* json_array_item_get_next(JSON_array_item* _item);
    JSON_array_item* json_array_item_get_previous(JSON_array_item* _item);

    JSON_array_item* json_array_append_value(JSON_array* _array, JSON_value* _value);
    JSON_array_item* json_array_append_object(JSON_array* _array);
    JSON_array_item* json_array_append_array(JSON_array* _array);
    JSON_array_item* json_array_append_mstring(JSON_array* _array, mstring _mstring);
    JSON_array_item* json_array_append_string(JSON_array* _array, const char* _string);
    JSON_array_item* json_array_append_number(JSON_array* _array, double _number);
    JSON_array_item* json_array_append_number_mstring(JSON_array* _array, mstring _mnumber);
    JSON_array_item* json_array_append_number_string(JSON_array* _array, const char* _number);
    JSON_array_item* json_array_append_boolean(JSON_array* _array, bool _boolean);
    JSON_array_item* json_array_append_null(JSON_array* _array);

    JSON_array_item* json_array_insert_value(JSON_array* _array, size_t _ith, JSON_value* _value);
    JSON_array_item* json_array_insert_object(JSON_array* _array, size_t _ith);
    JSON_array_item* json_array_insert_array(JSON_array* _array, size_t _ith);
    JSON_array_item* json_array_insert_mstring(JSON_array* _array, size_t _ith, mstring _mstring);
    JSON_array_item* json_array_insert_string(JSON_array* _array, size_t _ith, const char* _string);
    JSON_array_item* json_array_insert_number(JSON_array* _array, size_t _ith, double _number);
    JSON_array_item* json_array_insert_number_mstring(JSON_array* _array, size_t _ith, mstring _mstring);
    JSON_array_item* json_array_insert_number_string(JSON_array* _array, size_t _ith, const char* _number);
    JSON_array_item* json_array_insert_boolean(JSON_array* _array, size_t _ith, bool _boolean);
    JSON_array_item* json_array_insert_null(JSON_array* _array, size_t _ith);

    size_t json_array_get_length(JSON_array* _array);
    JSON_value* json_array_get_value_wrapper(JSON_array* _array);
    JSON_array_item* json_array_get_item_head(JSON_array* _array);
    JSON_array_item* json_array_get_item_tail(JSON_array* _array);

    JSON_array_item* json_array_get_array_item(JSON_array* _array, size_t _ith);
    JSON_value* json_array_get_value(JSON_array* _array, size_t _ith);
    JSON_type json_array_get_type(JSON_array* _array, size_t _ith);
    JSON_object* json_array_get_object(JSON_array* _array, size_t _ith);
    JSON_array* json_array_get_array(JSON_array* _array, size_t _ith);
    const char* json_array_get_string(JSON_array* _array, size_t _ith);
    double json_array_get_number(JSON_array* _array, size_t _ith);
    const char* json_array_get_number_string(JSON_array* _array, size_t _ith);
    double json_array_get_real_number(JSON_array* _array, size_t _ith);
    bool json_array_get_boolean(JSON_array* _array, size_t _ith);

    JSON_array* json_array_remove(JSON_array* _array, size_t _ith);
    JSON_array* json_array_remove_all(JSON_array* _array);

    mstring json_parse_mstring(size_t* _jj, const char* _content, size_t _position, size_t _length);
    mstring json_parse_number_mstring(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_object(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_array(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_string(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_number_string(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_boolean(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_null(size_t* _jj, const char* _content, size_t _position, size_t _length);
    JSON_value* json_parse_value(size_t* _jj, const char* _content, size_t position, size_t _length);

    char* json_read_file(const char* _filepath);
    JSON_value* json_parse_content(char* _content);
    JSON_value* json_parse_file(const char* _filepath);

    size_t json_print_get_length_buffer(JSON_value* _value, bool _pretty, size_t _indent);
    char* json_print_buffer(JSON_value* value, bool _pretty, size_t _indent);

    JSON_object_item* json_object_ms_get_object_item(JSON_object* _object, mstring _name);
    JSON_object_item* json_object_s_get_object_item(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_n_get_object_item(JSON_object* _object, double _name);
    JSON_object_item* json_object_nms_get_object_item(JSON_object* _object, mstring _name);
    JSON_object_item* json_object_ns_get_object_item(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_b_get_object_item(JSON_object* _object, bool _name);
    JSON_object_item* json_object_i_get_object_item(JSON_object* _object);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_JSON_H___
