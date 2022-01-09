#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmJson.h"
#include "../../mmLogSimple.h"


struct _JSON_object_item {
    JSON_name* name;
    JSON_value* value;
    struct _JSON_object_item* next;
    struct _JSON_object_item* previous;
};

struct _JSON_array_item {
    JSON_value* value;
    struct _JSON_array_item* next;
    struct _JSON_array_item* previous;
};

typedef union _JSON_u_name {
    char* string;
    double number;
    char* number_string;
    bool boolean;
} JSON_u_name;

struct _JSON_name {
    int type;
    JSON_u_name u;
};

typedef union _JSON_u_value {
    JSON_object* object;
    JSON_array* array;
    char* string;
    double number;
    char* number_string;
    bool boolean;
} JSON_u_value;

struct _JSON_value {
    int type;
    struct _JSON_value* parent;
    JSON_u_value u;
};

struct _JSON_object {
    JSON_value* value_wrapper;
    JSON_object_item* item_head;
    JSON_object_item* item_tail;
};

struct _JSON_array {
    JSON_value* value_wrapper;
    JSON_array_item* item_head;
    JSON_array_item* item_tail;
};


void* json_malloc(size_t _size)
{
    void* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}

char* json_mstrdup(const char* _content, size_t _length)
{
    char* string = (char*)memloc(_length + 1);
    if (string == NULL) return NULL;
    strncpy(string, _content, _length);
    string[_length] = '\0';

    return string;
}


JSON_name* json_name_create_mstring(mstring _mstring)
{
    if (_mstring.value == NULL) return JSON_DEFAULT_NAME;

    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    char* name_dup = json_mstrdup(_mstring.value, _mstring.length);
    if (name_dup == NULL) {
        memfree(name);
        return JSON_DEFAULT_NAME;
    }
    name->type = json_type_string;
    name->u.string = name_dup;
    return name;
}

JSON_name* json_name_create_string(const char* _string)
{
    if (_string == NULL) return JSON_DEFAULT_NAME;

    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    char* name_dup = json_mstrdup(_string, strlen(_string));
    if (name_dup == NULL) {
        memfree(name);
        return JSON_DEFAULT_NAME;
    }
    name->type = json_type_string;
    name->u.string = name_dup;
    return name;
}

JSON_name* json_name_create_number(double _number)
{
    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    name->type = json_type_number;
    name->u.number = _number;
    return name;
}

JSON_name* json_name_create_number_mstring(mstring _mnumber)
{
    if (_mnumber.value == NULL) return JSON_DEFAULT_NAME;

    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    char* number_dup = json_mstrdup(_mnumber.value, _mnumber.length);
    if (number_dup == NULL) {
        memfree(name);
        return JSON_DEFAULT_NAME;
    }
    name->type = json_type_number_string;
    name->u.number_string = number_dup;
    return name;
}

JSON_name* json_name_create_number_string(const char* _number)
{
    if (_number == NULL) return JSON_DEFAULT_NAME;

    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    char* number_dup = json_mstrdup(_number, strlen(_number));
    if (number_dup == NULL) {
        memfree(name);
        return JSON_DEFAULT_NAME;
    }
    name->type = json_type_number_string;
    name->u.number_string = number_dup;
    return name;
}

JSON_name* json_name_create_boolean(bool _boolean)
{
    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    name->type = json_type_boolean;
    name->u.boolean = _boolean;
    return name;
}

JSON_name* json_name_create_null(void)
{
    JSON_name* name = (JSON_name*)memloc(sizeof(JSON_name));
    if (name == NULL) return JSON_DEFAULT_NAME;
    name->type = json_type_null;
    return name;
}

void json_name_free(JSON_name* _name)
{
    if (_name->type == json_type_string && _name->u.string != NULL) {
        memfree(_name->u.string);
    }
    else if (_name->type == json_type_number) {
    }
    else if (_name->type == json_type_number_string && _name->u.number_string != NULL) {
        memfree(_name->u.number_string);
    }
    else if (_name->type == json_type_boolean) {
    }
    else if (_name->type == json_type_null) {
    }

    memfree(_name);
}


bool json_name_is_same(JSON_name* _name0, JSON_name* _name1)
{
    if (_name0->type == _name1->type) {
        if (_name0->type == json_type_string) {
            if (strcmp(_name0->u.string, _name1->u.string) == 0) return true;
        }
        else if (_name0->type == json_type_number) {
            if (_name0->u.number == _name1->u.number) return true;
        }
        else if (_name0->type == json_type_number_string) {
            if (strcmp(_name0->u.number_string, _name1->u.number_string) == 0) return true;
        }
        else if (_name0->type == json_type_boolean) {
            if (_name0->u.boolean == _name1->u.boolean) return true;
        }
        else if (_name0->type == json_type_null) {
            return true;
        }
    }

    return false;
}

JSON_name* json_name_duplicate(JSON_name* _name)
{
    if (_name == NULL) return JSON_DEFAULT_JNAME;
    if (_name->type == json_type_string) {
        return json_name_create_string(_name->u.string);
    }
    else if (_name->type == json_type_number) {
        return json_name_create_number(_name->u.number);
    }
    else if (_name->type == json_type_number_string) {
        return json_name_create_number_string(_name->u.number_string);
    }
    else if (_name->type == json_type_boolean) {
        return json_name_create_boolean(_name->u.boolean);
    }
    else if (_name->type == json_type_null) {
        return json_name_create_null();
    }

    return JSON_DEFAULT_JNAME;
}


JSON_name* json_object_item_get_jname(JSON_object_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_JNAME;
    return _item->name;
}

const char* json_object_item_get_name(JSON_object_item* _item)
{
    if (_item == NULL || _item->name == NULL) return JSON_DEFAULT_NAME;
    if (_item->name->type != json_type_string) return JSON_DEFAULT_NAME;
    return _item->name->u.string;
}

JSON_object_item* json_object_set_jname_value(JSON_object* _object, JSON_name* _name, JSON_value* _value)
{
    if (_object == NULL || _name == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    if (json_object_has_jname(_object, _name) == true) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = (JSON_object_item*)memloc(sizeof(JSON_object_item));
    if (item == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    _value->parent = _object->value_wrapper;
    item->name = _name;
    item->value = _value;

    if (_object->item_head == NULL) {
        _object->item_head = item;
        _object->item_tail = item;
    }
    else {
        _object->item_tail->next = item;
        item->previous = _object->item_tail;
        _object->item_tail = item;
    }
    return item;
}

JSON_object_item* json_object_set_ith_jname_value(JSON_object* _object, size_t _ith, JSON_name* _name, JSON_value* _value)
{
    if (_object == NULL || _name == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    if (json_object_has_jname(_object, _name) == true) return JSON_DEFAULT_OBJECT_ITEM;
    size_t length = json_object_get_length(_object);
    if (_ith > length) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = (JSON_object_item*)memloc(sizeof(JSON_object_item));
    if (item == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    _value->parent = _object->value_wrapper;
    item->name = _name;
    item->value = _value;

    if (length == 0) {
        _object->item_head = item;
        _object->item_tail = item;
    }
    else {
        if (_ith == 0) {
            JSON_object_item* item_next = json_object_get_ith_object_item(_object, _ith);
            item->next = item_next;
            item_next->previous = item;
            _object->item_head = item;
        }
        else if (_ith == length) {
            JSON_object_item* item_previous = json_object_get_ith_object_item(_object, _ith - 1);
            item->previous = item_previous;
            item_previous->next = item;
            _object->item_tail = item;
        }
        else {
            JSON_object_item* item_next = json_object_get_ith_object_item(_object, _ith);
            JSON_object_item* item_previous = json_object_get_ith_object_item(_object, _ith - 1);
            item->next = item_next;
            item->previous = item_previous;
            item_next->previous = item;
            item_previous->next = item;
        }
    }

    return item;
}

JSON_name* json_object_get_jname(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_JNAME;
    JSON_object_item* item = _object->item_head;
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (item == NULL) return JSON_DEFAULT_JNAME;;
        item = item->next;
    }
    if (item == NULL) return JSON_DEFAULT_JNAME;

    return item->name;
}

bool json_object_has_jname(JSON_object* _object, JSON_name* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_object_item* item = _object->item_head;
    while (item != NULL && item->name != NULL) {
        if (json_name_is_same(item->name, _name) == true) {
            return true;
        }
        item = item->next;
    }

    return false;
}

JSON_object_item* json_object_get_jname_object_item(JSON_object* _object, JSON_name* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = _object->item_head;
    while (item != NULL && item->name != NULL) {
        if (json_name_is_same(item->name, _name) == true) return item;
        item = item->next;
    }

    return JSON_DEFAULT_OBJECT_ITEM;
}


JSON_value* json_value_create_object(void)
{
    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    JSON_object* object = (JSON_object*)memloc(sizeof(JSON_object));
    if (object == NULL) {
        memfree(value);
        return JSON_DEFAULT_VALUE;
    }
    object->value_wrapper = value;
    value->type = json_type_object;
    value->u.object = object;
    return value;
}

JSON_value* json_value_create_array(void)
{
    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    JSON_array* array = (JSON_array*)memloc(sizeof(JSON_array));
    if (array == NULL) {
        memfree(value);
        return JSON_DEFAULT_VALUE;
    }
    array->value_wrapper = value;
    value->type = json_type_array;
    value->u.array = array;
    return value;
}

JSON_value* json_value_create_mstring(mstring _mstring)
{
    if (_mstring.value == NULL) return JSON_DEFAULT_VALUE;

    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    char* string_dup = json_mstrdup(_mstring.value, _mstring.length);
    if (string_dup == NULL) {
        memfree(value);
        return JSON_DEFAULT_VALUE;
    }
    value->type = json_type_string;
    value->u.string = string_dup;
    return value;
}

JSON_value* json_value_create_string(const char* _string)
{
    if (_string == NULL) return JSON_DEFAULT_VALUE;

    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    char* string_dup = json_mstrdup(_string, strlen(_string));
    if (string_dup == NULL) {
        memfree(value);
        return JSON_DEFAULT_VALUE;
    }
    value->type = json_type_string;
    value->u.string = string_dup;
    return value;
}

JSON_value* json_value_create_number(double _number)
{
    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    value->type = json_type_number;
    value->u.number = _number;
    return value;
}

JSON_value* json_value_create_number_mstring(mstring _mnumber)
{
    if (_mnumber.value == NULL) return JSON_DEFAULT_VALUE;

    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    char* number_dup = json_mstrdup(_mnumber.value, _mnumber.length);
    if (number_dup == NULL) {
        memfree(value);
        return JSON_DEFAULT_VALUE;
    }
    value->type = json_type_number_string;
    value->u.number_string = number_dup;
    return value;
}

JSON_value* json_value_create_number_string(const char* _number)
{
    if (_number == NULL) return JSON_DEFAULT_VALUE;

    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    char* number_dup = json_mstrdup(_number, strlen(_number));
    if (number_dup == NULL) {
        memfree(value);
        return JSON_DEFAULT_VALUE;
    }
    value->type = json_type_number_string;
    value->u.number_string = number_dup;
    return value;
}

JSON_value* json_value_create_boolean(bool _boolean)
{
    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    value->type = json_type_boolean;
    value->u.boolean = _boolean;
    return value;
}

JSON_value* json_value_create_null(void)
{
    JSON_value* value = (JSON_value*)memloc(sizeof(JSON_value));
    if (value == NULL) return JSON_DEFAULT_VALUE;
    value->type = json_type_null;
    return value;
}

static void json_array_item_free(JSON_array_item* _item)
{
    if (_item != NULL) {
        if (_item->value != NULL) {
            json_value_free(_item->value);
        }
        memfree(_item);
    }
}

static void json_object_item_free(JSON_object_item* _item)
{
    if (_item != NULL) {
        if (_item->name != NULL) {
            json_name_free(_item->name);
        }
        if (_item->value != NULL) {
            json_value_free(_item->value);
        }
        memfree(_item);
    }
}

static void json_array_free(JSON_array* _array)
{
    JSON_array_item* item = _array->item_head;
    while (item != NULL) {
        JSON_array_item* to_free = item;
        item = item->next;
        json_array_item_free(to_free);
    }
    memfree(_array);
}

static void json_object_free(JSON_object* _object)
{
    JSON_object_item* item = _object->item_head;
    while (item != NULL) {
        JSON_object_item* to_free = item;
        item = item->next;
        json_object_item_free(to_free);
    }
    memfree(_object);
}

void json_value_free(JSON_value* _value)
{
    if (_value == NULL) {
        if (_value->type == json_type_object && _value->u.object != NULL) {
            json_object_free(_value->u.object);
        }
        else if (_value->type == json_type_array && _value->u.array != NULL) {
            json_array_free(_value->u.array);
        }
        else if (_value->type == json_type_string && _value->u.string != NULL) {
            memfree(_value->u.string);
        }
        else if (_value->type == json_type_number) {
        }
        else if (_value->type == json_type_number_string && _value->u.number_string != NULL) {
            memfree(_value->u.number_string);
        }
        else if (_value->type == json_type_boolean) {
        }
        else if (_value->type == json_type_null) {
        }

        memfree(_value);
    }
}


JSON_value* json_value_duplicate(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_VALUE;
    JSON_value* value_duplicate = NULL;
    if (_value->type == json_type_object && _value->u.object != NULL) {
        value_duplicate = json_value_create_object();
        JSON_object* object_duplicate = json_value_get_object(value_duplicate);
        JSON_object* object = json_value_get_object(_value);
        for (JSON_object_item* item = object->item_head; item != NULL; item = item->next) {
            JSON_name* item_name_duplicate = json_name_duplicate(item->name);
            JSON_value* item_value_duplicate = json_value_duplicate(item->value);
            json_object_set_jname_value(object_duplicate, item_name_duplicate, item_value_duplicate);
        }
    }
    else if (_value->type == json_type_array && _value->u.array != NULL) {
        value_duplicate = json_value_create_array();
        JSON_array* array_duplicate = json_value_get_array(value_duplicate);
        JSON_array* array = json_value_get_array(_value);
        for (JSON_array_item* item = array->item_head; item != NULL; item = item->next) {
            JSON_value* item_value_duplicate = json_value_duplicate(item->value);
            json_array_append_value(array_duplicate, item_value_duplicate);
        }
    }
    else if (_value->type == json_type_string && _value->u.string != NULL) {
        value_duplicate = json_value_create_string(_value->u.string);
    }
    else if (_value->type == json_type_number) {
        value_duplicate = json_value_create_number(_value->u.number);
    }
    else if (_value->type == json_type_number_string) {
        value_duplicate = json_value_create_number_string(_value->u.number_string);
    }
    else if (_value->type == json_type_boolean) {
        value_duplicate = json_value_create_boolean(_value->u.boolean);
    }
    else if (_value->type == json_type_null) {
        value_duplicate = json_value_create_null();
    }

    return value_duplicate;
}

size_t json_value_get_depth(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_DEPTH;
    JSON_value* value_current = _value;
    size_t depth = 0;
    while (value_current != NULL) {
        ++depth;
        value_current = _value->parent;
    }

    return depth;
}


JSON_type json_value_get_type(JSON_value* _value)
{
    if (_value == NULL) return json_type_error;

    return _value->type;
}

JSON_value* json_value_get_parent(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_VALUE;

    return _value->parent;
}

JSON_object* json_value_get_object(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_OBJECT;
    if (_value->type == json_type_null) return JSON_OBJECT_NULL;
    else if (_value->type == json_type_object) return _value->u.object;

    return JSON_DEFAULT_OBJECT;
}

JSON_array* json_value_get_array(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_ARRAY;
    if (_value->type == json_type_null) return JSON_ARRAY_NULL;
    else if (_value->type == json_type_array) return _value->u.array;

    return JSON_DEFAULT_ARRAY;
}

const char* json_value_get_string(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_STRING;
    if (_value->type == json_type_null) return JSON_STRING_NULL;
    else if (_value->type == json_type_string) return _value->u.string;

    return JSON_DEFAULT_STRING;
}

double json_value_get_number(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_NUMBER;
    if (_value->type == json_type_null) return JSON_NUMBER_NULL;
    else if (_value->type == json_type_number) return _value->u.number;

    return JSON_DEFAULT_NUMBER;
}

const char* json_value_get_number_string(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_STRING;
    if (_value->type == json_type_null) return JSON_NUMBER_STRING_NULL;
    else if (_value->type == json_type_number_string) return _value->u.number_string;

    return JSON_DEFAULT_STRING;
}

double json_value_get_real_number(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_NUMBER;
    if (_value->type == json_type_null) return JSON_NUMBER_NULL;
    else if (_value->type == json_type_number) return _value->u.number;
    else if (_value->type == json_type_number_string) return strtod(_value->u.number_string, NULL);

    return JSON_DEFAULT_NUMBER;
}

bool json_value_get_boolean(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_BOOLEAN;
    if (_value->type == json_type_null) return JSON_BOOLEAN_NULL;
    else if (_value->type == json_type_boolean) return _value->u.boolean;

    return JSON_DEFAULT_BOOLEAN;
}
bool json_value_is_null(JSON_value* _value)
{
    if (_value == NULL) return JSON_DEFAULT_BOOLEAN;
    return _value->type == json_type_null;
}


JSON_value* json_object_item_get_value(JSON_object_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_VALUE;
    return _item->value;
}

JSON_object_item* json_object_item_get_next(JSON_object_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    return _item->next;
}

JSON_object_item* json_object_item_get_previous(JSON_object_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    return _item->previous;
}


JSON_object_item* json_object_set_value(JSON_object* _object, const char* _name, JSON_value* _value)
{
    if (_object == NULL || _name == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_string(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_set_object(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_array(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_mstring(JSON_object* _object, const char* _name, mstring _mstring)
{
    if (_object == NULL || _name == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_string(JSON_object* _object, const char* _name, const char* _string)
{
    if (_object == NULL || _name == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_number(JSON_object* _object, const char* _name, double _number)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_number_mstring(JSON_object* _object, const char* _name, mstring _mnumber)
{
    if (_object == NULL || _name == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_number_string(JSON_object* _object, const char* _name, const char* _number)
{
    if (_object == NULL || _name == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_boolean(JSON_object* _object, const char* _name, bool _boolean)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_null(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_set_ith_value(JSON_object* _object, size_t _ith, const char* _name, JSON_value* _value)
{
    if (_object == NULL || _name == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_string(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_set_ith_object(JSON_object* _object, size_t _ith, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_array(JSON_object* _object, size_t _ith, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mstring)
{
    if (_object == NULL || _name == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_string(JSON_object* _object, size_t _ith, const char* _name, const char* _string)
{
    if (_object == NULL || _name == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_number(JSON_object* _object, size_t _ith, const char* _name, double _number)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_number_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mnumber)
{
    if (_object == NULL || _name == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _name, const char* _number)
{
    if (_object == NULL || _name == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_boolean(JSON_object* _object, size_t _ith, const char* _name, bool _boolean)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_set_ith_null(JSON_object* _object, size_t _ith, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


size_t json_object_get_length(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_LENGTH;
    JSON_object_item* item = _object->item_head;
    size_t length = 0;
    while (item != NULL) {
        ++length;
        item = item->next;
    }
    return length;
}

const char* json_object_get_name(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_NAME;
    JSON_name* name = json_object_get_jname(_object, _ith);
    if (name == NULL) return JSON_DEFAULT_NAME;
    if (name->type != json_type_string) return JSON_DEFAULT_NAME;

    return name->u.string;
}

bool json_object_has_name(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_name name = { 0 };
    name.type = json_type_string;
    name.u.string = (char*)_name;
    bool has_name = json_object_has_jname(_object, &name);

    return has_name;
}

JSON_value* json_object_get_value_wrapper(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_VALUE;

    return _object->value_wrapper;
}

JSON_object_item* json_object_get_item_head(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    return _object->item_head;
}

JSON_object_item* json_object_get_item_tail(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    return _object->item_tail;
}


JSON_object_item* json_object_get_object_item(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name name = { 0 };
    name.type = json_type_string;
    name.u.string = (char*)_name;
    return json_object_get_jname_object_item(_object, &name);
}

JSON_value* json_object_get_value(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return item->value;
}

JSON_type json_object_get_type(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return value->type;
}

JSON_object* json_object_get_object(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_object(value);
}

JSON_array* json_object_get_array(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_array(value);
}

const char* json_object_get_string(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_string(value);
}

double json_object_get_number(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_number(value);
}

const char* json_object_get_number_string(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_number_string(value);
}

double json_object_get_real_number(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_real_number(value);
}

bool json_object_get_boolean(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_get_value(_object, _name);

    return json_value_get_boolean(value);
}

bool json_object_is_null(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_get_value(_object, _name);

    return value->type == json_type_null;
}


JSON_object_item* json_object_get_ith_object_item(JSON_object* _object, size_t _ith)
{
    if (_object == NULL || _object->item_head == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    if (_ith >= json_object_get_length(_object)) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = _object->item_head;
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (item == NULL) return JSON_DEFAULT_OBJECT_ITEM;
        item = item->next;
    }

    return item;
}

JSON_value* json_object_get_ith_value(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_get_ith_object_item(_object, _ith);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return item->value;
}

JSON_type json_object_get_ith_type(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_get_ith_value(_object, _ith);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return value->type;
}

JSON_object* json_object_get_ith_object(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_object(value);
}

JSON_array* json_object_get_ith_array(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_array(value);
}

const char* json_object_get_ith_string(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_string(value);
}

double json_object_get_ith_number(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_number(value);
}

const char* json_object_get_ith_number_string(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_number_string(value);
}

double json_object_get_ith_real_number(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_real_number(value);
}

bool json_object_get_ith_boolean(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return json_value_get_boolean(value);
}

bool json_object_is_ith_null(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_get_ith_value(_object, _ith);

    return value->type == json_type_null;
}


JSON_object* json_object_remove(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT;
    JSON_object_item* item = json_object_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_OBJECT;
    if (item->previous == NULL && item->next == NULL) {
        _object->item_head = NULL;
        _object->item_tail = NULL;
    }
    else if (item->previous != NULL && item->next == NULL) {
        item->previous->next = NULL;
        _object->item_tail = item->previous;
    }
    else if (item->previous == NULL && item->next != NULL) {
        item->next->previous = NULL;
        _object->item_head = item->next;
    }
    else {
        item->next->previous = item->previous;
        item->previous->next = item->next;
    }
    json_object_item_free(item);

    return _object;
}

JSON_object* json_object_remove_all(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT;
    JSON_object_item* item = _object->item_head;
    while (item != NULL) {
        JSON_object_item* item_next = item->next;
        json_object_item_free(item_next);
        item = item_next;
    }
    _object->item_head = NULL;
    _object->item_tail = NULL;

    return _object;
}


JSON_value* json_array_item_get_value(JSON_array_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_VALUE;
    return _item->value;
}

JSON_array_item* json_array_item_get_next(JSON_array_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    return _item->next;
}

JSON_array_item* json_array_item_get_previous(JSON_array_item* _item)
{
    if (_item == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    return _item->previous;
}


JSON_array_item* json_array_append_value(JSON_array* _array, JSON_value* _value)
{
    if (_array == NULL || _value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = (JSON_array_item*)memloc(sizeof(JSON_array_item));
    if (item == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    _value->parent = _array->value_wrapper;
    item->value = _value;
    if (_array->item_head == NULL) {
        _array->item_head = item;
        _array->item_tail = item;
    }
    else {
        _array->item_tail->next = item;
        item->previous = _array->item_tail;
        _array->item_tail = item;
    }
    return item;
}

JSON_array_item* json_array_append_object(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_array(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_mstring(JSON_array* _array, mstring _mstring)
{
    if (_array == NULL || _mstring.value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_string(JSON_array* _array, const char* _string)
{
    if (_array == NULL || _string == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_number(JSON_array* _array, double _number)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_number_mstring(JSON_array* _array, mstring _mnumber)
{
    if (_array == NULL || _mnumber.value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_number_string(JSON_array* _array, const char* _number)
{
    if (_array == NULL || _number == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_boolean(JSON_array* _array, bool _boolean)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_append_null(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_append_value(_array, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_array_item* json_array_insert_value(JSON_array* _array, size_t _ith, JSON_value* _value)
{
    if (_array == NULL || _value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    size_t length = json_array_get_length(_array);
    if (_ith > length) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = (JSON_array_item*)memloc(sizeof(JSON_array_item));
    if (item == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    _value->parent = _array->value_wrapper;
    item->value = _value;
    if (length == 0) {
        _array->item_head = item;
        _array->item_tail = item;
    }
    else {
        if (_ith == 0) {
            item->next = _array->item_head;
            _array->item_head->previous = item;
            _array->item_head = item;
        }
        else if (_ith == length) {
            item->previous = _array->item_tail;
            _array->item_tail->next = item;
            _array->item_tail = item;
        }
        else {
            JSON_array_item* item_previous = json_array_get_array_item(_array, length - 1);
            JSON_array_item* item_next = json_array_get_array_item(_array, length);
            item->previous = item_previous;
            item_previous->next = item;
            item->next = item_next;
            item_next->previous = item;
        }
    }

    return item;
}

JSON_array_item* json_array_insert_object(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_array(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_mstring(JSON_array* _array, size_t _ith, mstring _mstring)
{
    if (_array == NULL || _mstring.value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_string(JSON_array* _array, size_t _ith, const char* _string)
{
    if (_array == NULL || _string == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_number(JSON_array* _array, size_t _ith, double _number)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_number_mstring(JSON_array* _array, size_t _ith, mstring _mnumber)
{
    if (_array == NULL || _mnumber.value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_number_string(JSON_array* _array, size_t _ith, const char* _number)
{
    if (_array == NULL || _number == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_boolean(JSON_array* _array, size_t _ith, bool _boolean)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_array_item* json_array_insert_null(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = json_array_insert_value(_array, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}


size_t json_array_get_length(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_LENGTH;
    JSON_array_item* item = _array->item_head;
    size_t length = 0;
    while (item != NULL) {
        ++length;
        item = item->next;
    }
    return length;
}

JSON_value* json_array_get_value_wrapper(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_VALUE;

    return _array->value_wrapper;
}

JSON_array_item* json_array_get_item_head(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;

    return _array->item_head;
}

JSON_array_item* json_array_get_item_tail(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;

    return _array->item_tail;
}


JSON_array_item* json_array_get_array_item(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY_ITEM;
    JSON_array_item* item = _array->item_head;
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (item == NULL) return JSON_DEFAULT_ARRAY_ITEM;
        item = item->next;
    }
    if (item == NULL) return JSON_DEFAULT_ARRAY_ITEM;

    return item;
}

JSON_value* json_array_get_value(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_VALUE;
    JSON_array_item* item = json_array_get_array_item(_array, _ith);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return item->value;
}

JSON_type json_array_get_type(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_array_get_value(_array, _ith);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return value->type;
}

JSON_object* json_array_get_object(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_object(value);
}

JSON_array* json_array_get_array(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_array(value);
}

const char* json_array_get_string(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_string(value);
}

double json_array_get_number(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_number(value);
    if (value == NULL || value->type != json_type_number) return JSON_DEFAULT_NUMBER;

    return value->u.number;
}

const char* json_array_get_number_string(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_number_string(value);
}

double json_array_get_real_number(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_real_number(value);
}

bool json_array_get_boolean(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_array_get_value(_array, _ith);

    return json_value_get_boolean(value);
}


JSON_array* json_array_remove(JSON_array* _array, size_t _ith)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY;
    JSON_array_item* item = json_array_get_array_item(_array, _ith);
    if (item == NULL) return JSON_DEFAULT_ARRAY;
    if (item->previous == NULL && item->next == NULL) {
        _array->item_head = NULL;
        _array->item_tail = NULL;
    }
    else if (item->previous != NULL && item->next == NULL) {
        item->previous->next = NULL;
        _array->item_tail = item->previous;
    }
    else if (item->previous == NULL && item->next != NULL) {
        item->next->previous = NULL;
        _array->item_head = item->next;
    }
    else {
        item->next->previous = item->previous;
        item->previous->next = item->next;
    }
    json_array_item_free(item);

    return _array;
}

JSON_array* json_array_remove_all(JSON_array* _array)
{
    if (_array == NULL) return JSON_DEFAULT_ARRAY;
    JSON_array_item* item = _array->item_head;
    while (item != NULL) {
        JSON_array_item* item_next = item->next;
        json_array_item_free(item_next);
        item = item_next;
    }
    _array->item_head = NULL;
    _array->item_tail = NULL;

    return _array;
}


static bool json_is_whitespace(const char* _content, size_t _position)
{
    if (_content[_position] == ' ' || _content[_position] == '\n' || _content[_position] == '\r' || _content[_position] == '\t' ) return true;
    else return false;
}

mstring json_parse_mstring(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    if (_content[_position] != '\"') {
        return JSON_DEFAULT_MSTRING;
    }

    int STATE = JSON_PARSE_STRING_OPEN;

    size_t ii = 0;
    for (ii = _position + 1; ii < _length; ++ii) {
        if (STATE == JSON_PARSE_STRING_OPEN) {
            if (_content[ii] == '\\') {
                STATE = JSON_PARSE_STRING_CONTROL_CHARACTER;
            }
            else if (_content[ii] == '\"') {
                STATE = JSON_PARSE_STRING_CLOSE;
                break;
            }
        }
        else if (STATE == JSON_PARSE_STRING_CLOSE) {
        }
        else if (STATE == JSON_PARSE_STRING_CONTROL_CHARACTER) {
            if (_content[ii] == '\"' || _content[ii] == '\\' || _content[ii] == '/' || _content[ii] == '\b' || _content[ii] == '\f' || _content[ii] == '\n' || _content[ii] == '\r' || _content[ii] == '\t') {
                STATE = JSON_PARSE_STRING_OPEN;
            }
            else if (_content[ii] == 'u') {
                STATE = JSON_PARSE_STRING_UTF;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_STRING_UTF) {
            size_t size_unicode = 4;
            for (size_t jj = 0; jj < size_unicode; ++jj) {
                if ((_content[ii + jj] < '0' || _content[ii + jj] > '9') && (_content[ii + jj] < 'a' || _content[ii + jj] > 'f') && (_content[ii + jj] < 'A' || _content[ii + jj] > 'F')) {
                    return JSON_DEFAULT_MSTRING;
                }
            }
            ii += size_unicode - 1;
            STATE = JSON_PARSE_STRING_OPEN;
        }
        else {
            return JSON_DEFAULT_MSTRING;
        }
    }

    if (_jj != NULL) {
        *_jj = ii;
    }

    return (mstring){ (char*)&_content[_position + 1], ii - _position - 1 };
}

mstring json_parse_number_mstring(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    int STATE = JSON_PARSE_NUMBER_OPEN;

    if (_content[_position] == '-') {
        STATE = JSON_PARSE_NUMBER_INTEGER_MINUS;
    }
    else if (_content[_position] == '+') {
        STATE = JSON_PARSE_NUMBER_INTEGER_PLUS;
    }
    else if (_content[_position] == '0') {
        STATE = JSON_PARSE_NUMBER_INTEGER_ZERO;
    }
    else if (_content[_position] >= '1' && _content[_position] <= '9') {
        STATE = JSON_PARSE_NUMBER_INTEGER_NON_ZERO;
    }
    else {
        return JSON_DEFAULT_MSTRING;
    }

    size_t ii = 0;
    for (ii = _position + 1; ii < _length; ++ii) {
        if (STATE == JSON_PARSE_NUMBER_OPEN) {
        }
        else if (STATE == JSON_PARSE_NUMBER_CLOSE) {
        }
        else if (STATE == JSON_PARSE_NUMBER_INTEGER_MINUS || STATE == JSON_PARSE_NUMBER_INTEGER_PLUS) {
            if (_content[ii] == '0') {
                STATE = JSON_PARSE_NUMBER_INTEGER_ZERO;
            }
            else if (_content[ii] >= '1' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_INTEGER_NON_ZERO;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_INTEGER_ZERO) {
            if (json_is_whitespace(_content, ii) == true || _content[ii] == ',' || _content[ii] == '}' || _content[ii] == ']') {
                STATE = JSON_PARSE_NUMBER_CLOSE;
                break;
            }
            else if (_content[ii] == '.') {
                STATE = JSON_PARSE_NUMBER_FRACTION_DOT;
            }
            else if (_content[ii] == 'E' || _content[ii] == 'e') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_E;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_INTEGER_NON_ZERO) {
            if (json_is_whitespace(_content, ii) == true || _content[ii] == ',' || _content[ii] == '}' || _content[ii] == ']') {
                STATE = JSON_PARSE_NUMBER_CLOSE;
                break;
            }
            else if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_INTEGER_DIGIT;
            }
            else if (_content[ii] == '.') {
                STATE = JSON_PARSE_NUMBER_FRACTION_DOT;
            }
            else if (_content[ii] == 'E' || _content[ii] == 'e') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_E;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_INTEGER_DIGIT) {
            if (json_is_whitespace(_content, ii) == true || _content[ii] == ',' || _content[ii] == '}' || _content[ii] == ']') {
                STATE = JSON_PARSE_NUMBER_CLOSE;
                break;
            }
            else if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_INTEGER_DIGIT;
            }
            else if (_content[ii] == '.') {
                STATE = JSON_PARSE_NUMBER_FRACTION_DOT;
            }
            else if (_content[ii] == 'E' || _content[ii] == 'e') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_E;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_FRACTION_DOT) {
            if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_FRACTION_DIGIT;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_FRACTION_DIGIT) {
            if (json_is_whitespace(_content, ii) == true || _content[ii] == ',' || _content[ii] == '}' || _content[ii] == ']') {
                STATE = JSON_PARSE_NUMBER_CLOSE;
                break;
            }
            else if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_FRACTION_DIGIT;
            }
            else if (_content[ii] == 'E' || _content[ii] == 'e') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_E;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_EXPONENT_E) {
            if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_DIGIT;
            }
            else if (_content[ii] == '+' || _content[ii] == '-') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_SIGN;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_EXPONENT_SIGN) {
            if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_DIGIT;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else if (STATE == JSON_PARSE_NUMBER_EXPONENT_DIGIT) {
            if (json_is_whitespace(_content, ii) == true || _content[ii] == ',' || _content[ii] == '}' || _content[ii] == ']') {
                STATE = JSON_PARSE_NUMBER_CLOSE;
                break;
            }
            else if (_content[ii] >= '0' && _content[ii] <= '9') {
                STATE = JSON_PARSE_NUMBER_EXPONENT_DIGIT;
            }
            else {
                return JSON_DEFAULT_MSTRING;
            }
        }
        else {
            return JSON_DEFAULT_MSTRING;
        }
    }

    if (_jj != NULL) {
        *_jj = ii - 1;
    }

    return (mstring){ (char*)&_content[_position], ii - _position };
}

JSON_value* json_parse_object(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    if (_content[_position] != '{') {
        return JSON_DEFAULT_VALUE;
    }

    int STATE = JSON_PARSE_OBJECT_OPEN;

    JSON_value* value = json_value_create_object();
    JSON_object* object = json_value_get_object(value);
    JSON_name* name = NULL;

    size_t ii = 0;
    for (ii = _position + 1; ii < _length; ++ii) {
        if (json_is_whitespace(_content, ii) == true) {
            continue;
        }

        if (STATE == JSON_PARSE_OBJECT_OPEN) {
            if (_content[ii] == '\"') {
                STATE = JSON_PARSE_OBJECT_NAME;
                size_t kk = 0;
                mstring mstr = json_parse_mstring(&kk, _content, ii, _length);
                name = json_name_create_mstring(mstr);
                ii = kk;
                if (name == NULL) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
            }
            else if ((_content[ii] >= '0' && _content[ii] <= '9') || _content[ii] == '-' || _content[ii] == '+') {
                STATE = JSON_PARSE_OBJECT_NAME;
                size_t kk = 0;
                mstring mnumber = json_parse_number_mstring(&kk, _content, ii, _length);
                name = json_name_create_number_mstring(mnumber);
                ii = kk;
                if (name == NULL) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
            }
            else if (_content[ii] == 't') {
                STATE = JSON_PARSE_OBJECT_NAME;
                const char* right = JSON_PHRASE_TRUE;
                if (strncmp(right, &_content[ii], strlen(right)) != 0) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
                name = json_name_create_boolean(true);
                ii += strlen(right) - 1;
                if (name == NULL) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
            }
            else if (_content[ii] == 'f') {
                STATE = JSON_PARSE_OBJECT_NAME;
                const char* wrong = JSON_PHRASE_FALSE;
                if (strncmp(wrong, &_content[ii], strlen(wrong)) != 0) {
                    return JSON_DEFAULT_VALUE;
                }
                name = json_name_create_boolean(false);
                ii += strlen(wrong) - 1;
                if (name == NULL) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
            }
            else if (_content[ii] == 'n') {
                STATE = JSON_PARSE_OBJECT_NAME;
                const char* invalid = JSON_PHRASE_FALSE;
                if (strncmp(invalid, &_content[ii], strlen(invalid)) != 0) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
                name = json_name_create_null();
                ii += strlen(invalid) - 1;
                if (name == NULL) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
            }
            else if (_content[ii] == '}') {
                STATE = JSON_PARSE_OBJECT_CLOSE;
                break;
            }
            else {
                json_value_free(value);
                return JSON_DEFAULT_VALUE;
            }
        }
        else if (STATE == JSON_PARSE_OBJECT_CLOSE) {
        }
        else if (STATE == JSON_PARSE_OBJECT_NAME) {
            if (_content[ii] == ':') {
                STATE = JSON_PARSE_OBJECT_COLON;
            }
            else {
                json_name_free(name);
                json_value_free(value);
                return JSON_DEFAULT_VALUE;
            }
        }
        else if (STATE == JSON_PARSE_OBJECT_COLON) {
            STATE = JSON_PARSE_OBJECT_VALUE;
            size_t kk = 0;
            JSON_value* value_child = json_parse_value(&kk, _content, ii, _length);
            value_child->parent = value;
            if (value_child == NULL) {
                json_name_free(name);
                json_value_free(value);
                return JSON_DEFAULT_VALUE;
            }
            ii = kk;
            JSON_object_item* item = (JSON_object_item*)memloc(sizeof(JSON_object_item));
            if (item == NULL) {
                json_name_free(name);
                json_value_free(value);
                return JSON_DEFAULT_VALUE;
            }
            item->name = name;
            item->value = value_child;
            if (object->item_head == NULL) {
                object->item_head = item;
                object->item_tail = item;
            }
            else {
                object->item_tail->next = item;
                item->previous = object->item_tail;
                object->item_tail = item;
            }
        }
        else if (STATE == JSON_PARSE_OBJECT_VALUE) {
            if (_content[ii] == ',') {
                STATE = JSON_PARSE_OBJECT_OPEN;
            }
            else if (_content[ii] == '}') {
                STATE = JSON_PARSE_OBJECT_CLOSE;
                break;
            }
            else {
                json_value_free(value);
                return JSON_DEFAULT_VALUE;
            }
        }
        else {
            return JSON_DEFAULT_VALUE;
        }
    }

    if (_jj != NULL) {
        *_jj = ii;
    }

    return value;
}

JSON_value* json_parse_array(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    if (_content[_position] != '[') {
        return JSON_DEFAULT_VALUE;
    }

    int STATE = JSON_PARSE_ARRAY_OPEN;

    JSON_value* value = json_value_create_array();
    JSON_array* array = json_value_get_array(value);

    size_t ii = 0;
    for (ii = _position + 1; ii < _length; ++ii) {
        if (json_is_whitespace(_content, ii) == true) {
            continue;
        }

        if (STATE == JSON_PARSE_ARRAY_OPEN) {
            if (_content[ii] == ']') {
                STATE = JSON_PARSE_ARRAY_CLOSE;
                break;
            }
            else {
                STATE = JSON_PARSE_ARRAY_VALUE;
                size_t kk = 0;
                JSON_value* value_child = json_parse_value(&kk, _content, ii, _length);
                value_child->parent = value;
                ii = kk;
                JSON_array_item* item = (JSON_array_item*)memloc(sizeof(JSON_array_item));
                if (item == NULL) {
                    json_value_free(value);
                    return JSON_DEFAULT_VALUE;
                }
                item->value = value_child;
                if (array->item_head == NULL) {
                    array->item_head = item;
                    array->item_tail = item;
                }
                else {
                    array->item_tail->next = item;
                    item->previous = array->item_tail;
                    array->item_tail = item;
                }
            }
        }
        else if (STATE == JSON_PARSE_ARRAY_CLOSE) {
        }
        else if (STATE == JSON_PARSE_ARRAY_VALUE) {
            if (_content[ii] == ',') {
                STATE = JSON_PARSE_ARRAY_OPEN;
            }
            else if (_content[ii] == ']') {
                STATE = JSON_PARSE_ARRAY_CLOSE;
                break;
            }
            else {
                return JSON_DEFAULT_VALUE;
            }
        }
        else {
            return JSON_DEFAULT_VALUE;
        }
    }

    if (_jj != NULL) {
        *_jj = ii;
    }

    return value;
}

JSON_value* json_parse_string(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    mstring mstr = json_parse_mstring(_jj, _content, _position, _length);
    if (mstr.value == NULL) {
        return JSON_DEFAULT_VALUE;
    }

    JSON_value* value = json_value_create_mstring(mstr);

    return value;
}

JSON_value* json_parse_number_string(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    mstring mnumber = json_parse_number_mstring(_jj, _content, _position, _length);
    if (mnumber.value == NULL) {
        return JSON_DEFAULT_VALUE;
    }

    JSON_value* value = json_value_create_number_mstring(mnumber);

    return value;
}

JSON_value* json_parse_boolean(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    size_t ii = 0;
    for (ii = _position; ii < _length; ++ii) {
        if (_content[ii] != 't' && _content[ii] != 'f') {
            return JSON_DEFAULT_VALUE;
        }

        if (_content[ii] == 't') {
            const char* right = JSON_PHRASE_TRUE;
            if (strncmp(right, &_content[ii], strlen(right)) != 0) {
                return JSON_DEFAULT_VALUE;
            }

            JSON_value* value = json_value_create_boolean(true);

            if (_jj != NULL) {
                *_jj = ii + strlen(right) - 1;
            }

            return value;
        }
        else if (_content[ii] == 'f') {
            const char* wrong = JSON_PHRASE_FALSE;
            if (strncmp(wrong, &_content[ii], strlen(wrong)) != 0) {
                return JSON_DEFAULT_VALUE;
            }

            JSON_value* value = json_value_create_boolean(false);

            if (_jj != NULL) {
                *_jj = ii + strlen(wrong) - 1;
            }

            return value;
        }
    }

    return JSON_DEFAULT_VALUE;
}

JSON_value* json_parse_null(size_t* _jj, const char* _content, size_t _position, size_t _length)
{
    size_t ii = 0;
    for (ii = _position; ii < _length; ++ii) {
        if (_content[ii] != 'n') {
            return JSON_DEFAULT_VALUE;
        }

        const char* invalid = JSON_PHRASE_NULL;
        if (strncmp(invalid, &_content[ii], strlen(invalid)) != 0) {
            return JSON_DEFAULT_VALUE;
        }

        JSON_value* value = json_value_create_null();

        if (_jj != NULL) {
            *_jj = ii + strlen(invalid) - 1;
        }

        return value;
    }

    return JSON_DEFAULT_VALUE;
}

JSON_value* json_parse_value(size_t* _jj, const char* _content, size_t position, size_t _length)
{
    size_t ii = 0;
    for  (ii = position; ii < _length; ++ii) {
        if (json_is_whitespace(_content, ii) == false) {
            break;
        }
    }

    JSON_value* value = NULL;
    if (_content[ii] == '{') {
        value = json_parse_object(_jj, _content, ii, _length);
    }
    else if (_content[ii] == '[') {
        value = json_parse_array(_jj, _content, ii, _length);
    }
    else if (_content[ii] == '\"') {
        value = json_parse_string(_jj, _content, ii, _length);
    }
    else if ((_content[ii] >= '0' && _content[ii] <= '9') || _content[ii] == '-' || _content[ii] == '+') {
        value = json_parse_number_string(_jj, _content, ii, _length);
    }
    else if (_content[ii] == 't' || _content[ii] == 'f') {
        value = json_parse_boolean(_jj, _content, ii, _length);
    }
    else if (_content[ii] == 'n') {
        value = json_parse_null(_jj, _content, ii, _length);
    }
    else {
        return JSON_DEFAULT_VALUE;
    }

    return value;
}


char* json_read_file(const char* _filepath)
{
    FILE *fp = fopen(_filepath, "r");
    if (!fp) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long position = ftell(fp);
    if (position < 0) {
        fclose(fp);
        return NULL;
    }
    rewind(fp);

    size_t length = position;
    char* content = (char*)memloc(sizeof(char) * (length + 1));
    size_t nread = fread(content, 1, length, fp);
    if (nread == 0 || ferror(fp)) {
        fclose(fp);
        memfree(content);
        return NULL;
    }

    fclose(fp);
    content[nread] = '\0';

    return content;
}

JSON_value* json_parse_content(char* _content)
{
    size_t ii_new = 0;
    JSON_value* value = json_parse_value(&ii_new, _content, 0, strlen(_content));
    while (++ii_new < strlen(_content)) {
        if (json_is_whitespace(_content, ii_new) == false) {
            if (value != NULL) {
                json_value_free(value);
            }
            return NULL;
        }
    }

    return value;
}

JSON_value* json_parse_file(const char* _filepath)
{
    char* content = json_read_file(_filepath);
    if (content == NULL) {
        return NULL;
    }

    JSON_value* value = json_parse_content(content);
    memfree(content);

    return value;
}


static size_t json_print_get_length_value(JSON_value* _value, bool _pretty, size_t _indent, size_t _depth);
static size_t json_print_get_length_object(JSON_object* _object, bool _pretty, size_t _indent, size_t _depth);
static size_t json_print_get_length_array(JSON_array* _array, bool _pretty, size_t _indent, size_t _depth);
static void json_print_buffer_indent(char* _buffer, size_t _indent, size_t _depth);
static void json_print_buffer_value(char* _buffer, JSON_value* _value, bool _pretty, size_t _indent, size_t _depth);
static void json_print_buffer_object(char* _buffer, JSON_object* _object, bool _pretty, size_t _indent, size_t _depth);
static void json_print_buffer_array(char* _buffer, JSON_array* _array, bool _pretty, size_t _indent, size_t _depth);

static size_t json_print_get_length_value(JSON_value* _value, bool _pretty, size_t _indent, size_t _depth)
{
    if (_value == NULL) {
        return 0;
    }
    size_t length = 0;
    if (_value->type == json_type_object && _value->u.object != NULL) {
        length += json_print_get_length_object(_value->u.object, _pretty, _indent, _depth + 1);
    }
    else if (_value->type == json_type_array && _value->u.array != NULL) {
        length += json_print_get_length_array(_value->u.array, _pretty, _indent, _depth + 1);
    }
    else if (_value->type == json_type_string && _value->u.string != NULL) {
        length += strlen(_value->u.string) + strlen("\"\"");
    }
    else if (_value->type == json_type_number) {
        char number[JSON_SIZE_BUFFER_NUMBER] = { 0, };
        memset(number, 0x00, JSON_SIZE_BUFFER_NUMBER);
        sprintf(number, JSON_FORMAT_NUMBER, _value->u.number);
        length += strlen(number);
    }
    else if (_value->type == json_type_number_string && _value->u.number_string != NULL) {
        length += strlen(_value->u.number_string);
    }
    else if (_value->type == json_type_boolean) {
        if (_value->u.boolean == true) {
            length += strlen(JSON_PHRASE_TRUE);
        }
        else {
            length += strlen(JSON_PHRASE_FALSE);
        }
    }
    else if (_value->type == json_type_null) {
        length += strlen(JSON_PHRASE_FALSE);
    }

    return length;

}

static size_t json_print_get_length_object(JSON_object* _object, bool _pretty, size_t _indent, size_t _depth)
{
    size_t length = strlen("{}");
    if (_pretty == true) {
        length += strlen("\n") + _indent * (_depth - 1);
    }
    JSON_object_item* item = _object->item_head;
    while (item != NULL && item->name != NULL) {
        if (_pretty == true) {
            length += _indent * _depth;
        }
        if (item->name->type == json_type_string) {
            length += strlen(item->name->u.string) + strlen("\"\":");
        }
        else if (item->name->type == json_type_number) {
            char number[JSON_SIZE_BUFFER_NUMBER] = { 0, };
            memset(number, 0x00, JSON_SIZE_BUFFER_NUMBER);
            sprintf(number, JSON_FORMAT_NUMBER, item->name->u.number);
            length += strlen(number) + strlen(":");
        }
        else if (item->name->type == json_type_number_string) {
            length += strlen(item->name->u.number_string) + strlen(":");
        }
        else if (item->name->type == json_type_boolean) {
            if (item->name->u.boolean == true) {
                length += strlen(JSON_PHRASE_TRUE) + strlen(":");
            }
            else {
                length += strlen(JSON_PHRASE_FALSE) + strlen(":");
            }
        }
        else if (item->name->type == json_type_null) {
            length += strlen(JSON_PHRASE_FALSE) + strlen(":");
        }
        else {
            return JSON_DEFAULT_LENGTH;
        }

        if (_pretty == true) {
            length += strlen(" ");
        }
        length += json_print_get_length_value(item->value, _pretty, _indent, _depth);
        if (item->next != NULL) {
            length += strlen(",");
        }
        if (_pretty == true) {
            length += strlen("\n");
        }
        item = item->next;
    }

    return length;
}

static size_t json_print_get_length_array(JSON_array* _array, bool _pretty, size_t _indent, size_t _depth)
{
    size_t length = strlen("[]");
    if (_pretty == true) {
        length += strlen("\n") + _indent * (_depth - 1);
    }
    JSON_array_item* item = _array->item_head;
    while (item != NULL) {
        if (_pretty == true) {
            length += _indent * _depth;
        }
        length += json_print_get_length_value(item->value, _pretty, _indent, _depth);
        if (item->next != NULL) {
            length += strlen(",");
        }
        if (_pretty == true) {
            length += strlen("\n");
        }
        item = item->next;
    }

    return length;
}

size_t json_print_get_length_buffer(JSON_value* _value, bool _pretty, size_t _indent)
{
    return json_print_get_length_value(_value, _pretty, _indent, 0);
}

static void json_print_buffer_indent(char* _buffer, size_t _indent, size_t _depth)
{
    for (size_t ii = 0; ii < _indent * _depth; ++ii) {
        sprintf(&_buffer[strlen(_buffer)], " ");
    }
}

static void json_print_buffer_value(char* _buffer, JSON_value* _value, bool _pretty, size_t _indent, size_t _depth)
{
    if (_value->type == json_type_object && _value->u.object != NULL) {
        json_print_buffer_object(_buffer, _value->u.object, _pretty, _indent, _depth + 1);
    }
    else if (_value->type == json_type_array && _value->u.array != NULL) {
        json_print_buffer_array(_buffer, _value->u.array, _pretty, _indent, _depth + 1);
    }
    else if (_value->type == json_type_string && _value->u.string != NULL) {
        sprintf(&_buffer[strlen(_buffer)], "\"%s\"", _value->u.string);
    }
    else if (_value->type == json_type_number) {
        sprintf(&_buffer[strlen(_buffer)], JSON_FORMAT_NUMBER, _value->u.number);
    }
    else if (_value->type == json_type_number_string && _value->u.number_string != NULL) {
        sprintf(&_buffer[strlen(_buffer)], "%s", _value->u.number_string);
    }
    else if (_value->type == json_type_boolean) {
        if (_value->u.boolean == true) {
            sprintf(&_buffer[strlen(_buffer)], JSON_PHRASE_TRUE);
        }
        else {
            sprintf(&_buffer[strlen(_buffer)], JSON_PHRASE_FALSE);
        }
    }
    else if (_value->type == json_type_null) {
        sprintf(&_buffer[strlen(_buffer)], JSON_PHRASE_FALSE);
    }
}

static void json_print_buffer_object(char* _buffer, JSON_object* _object, bool _pretty, size_t _indent, size_t _depth)
{
    sprintf(&_buffer[strlen(_buffer)], "{");
    if (_pretty == true) {
        sprintf(&_buffer[strlen(_buffer)], "\n");
    }
    JSON_object_item* item = _object->item_head;
    while (item != NULL && item->name != NULL) {
        if (_pretty == true) {
            json_print_buffer_indent(_buffer, _indent, _depth);
        }
        if (item->name->type == json_type_string) {
            sprintf(&_buffer[strlen(_buffer)], "\"%s\":", item->name->u.string);
        }
        else if (item->name->type == json_type_number) {
            sprintf(&_buffer[strlen(_buffer)], JSON_FORMAT_NUMBER, item->name->u.number);
        }
        else if (item->name->type == json_type_number_string) {
            sprintf(&_buffer[strlen(_buffer)], "%s:", item->name->u.number_string);
        }
        else if (item->name->type == json_type_boolean) {
            if (item->name->u.boolean == true) {
                sprintf(&_buffer[strlen(_buffer)], "%s:", JSON_PHRASE_TRUE);
            }
            else {
                sprintf(&_buffer[strlen(_buffer)], "%s:", JSON_PHRASE_FALSE);
            }
        }
        else if (item->name->type == json_type_null) {
            sprintf(&_buffer[strlen(_buffer)], "%s:", JSON_PHRASE_FALSE);
        }
        else {
            return;
        }

        if (_pretty == true) {
            sprintf(&_buffer[strlen(_buffer)], " ");
        }
        json_print_buffer_value(_buffer, item->value, _pretty, _indent, _depth);
        if (item->next != NULL) {
            sprintf(&_buffer[strlen(_buffer)], ",");
        }
        if (_pretty == true) {
            sprintf(&_buffer[strlen(_buffer)], "\n");
        }
        item = item->next;
    }
    if (_pretty == true) {
        json_print_buffer_indent(_buffer, _indent, _depth - 1);
    }
    sprintf(&_buffer[strlen(_buffer)], "}");
}

static void json_print_buffer_array(char* _buffer, JSON_array* _array, bool _pretty, size_t _indent, size_t _depth)
{
    sprintf(&_buffer[strlen(_buffer)], "[");
    if (_pretty == true) {
        sprintf(&_buffer[strlen(_buffer)], "\n");
    }
    JSON_array_item* item = _array->item_head;
    while (item != NULL) {
        if (_pretty == true) {
            json_print_buffer_indent(_buffer, _indent, _depth);
        }
        json_print_buffer_value(_buffer, item->value, _pretty, _indent, _depth);
        if (item->next != NULL) {
            sprintf(&_buffer[strlen(_buffer)], ",");
        }
        if (_pretty == true) {
            sprintf(&_buffer[strlen(_buffer)], "\n");
        }
        item = item->next;
    }
    if (_pretty == true) {
        json_print_buffer_indent(_buffer, _indent, _depth - 1);
    }
    sprintf(&_buffer[strlen(_buffer)], "]");
}

char* json_print_buffer(JSON_value* value, bool _pretty, size_t _indent)
{
    size_t length = json_print_get_length_buffer(value, _pretty, _indent);
    char* buffer = (char*)memloc(length + 1);
    json_print_buffer_value(buffer, value, _pretty, _indent, 0);

    return buffer;
}


JSON_object_item* json_object_ms_get_object_item(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    char n[_name.length + 1];
    memset(n, 0x00, _name.length + 1);
    snprintf(n, _name.length + 1, "%s", _name.value);
    JSON_name name = { 0 };
    name.type = json_type_string;
    name.u.string = n;
    return json_object_get_jname_object_item(_object, &name);
}

JSON_object_item* json_object_s_get_object_item(JSON_object* _object, const char* _name)
{
    return json_object_get_object_item(_object, _name);
}

JSON_object_item* json_object_n_get_object_item(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name name = { 0 };
    name.type = json_type_number;
    name.u.number = _name;
    return json_object_get_jname_object_item(_object, &name);
}

JSON_object_item* json_object_nms_get_object_item(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    char n[_name.length + 1];
    memset(n, 0x00, _name.length + 1);
    snprintf(n, _name.length + 1, "%s", _name.value);
    JSON_name name = { 0 };
    name.type = json_type_number_string;
    name.u.number_string = n;
    return json_object_get_jname_object_item(_object, &name);
}

JSON_object_item* json_object_ns_get_object_item(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name name = { 0 };
    name.type = json_type_number_string;
    name.u.number_string = (char*)_name;
    return json_object_get_jname_object_item(_object, &name);
}

JSON_object_item* json_object_b_get_object_item(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name name = { 0 };
    name.type = json_type_boolean;
    name.u.boolean = _name;
    return json_object_get_jname_object_item(_object, &name);
}

JSON_object_item* json_object_i_get_object_item(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name name = { 0 };
    name.type = json_type_null;
    return json_object_get_jname_object_item(_object, &name);
}
