#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmJsonE.h"
#include "../../mmLogSimple.h"


JSON_object_item* json_object_ms_set_value(JSON_object* _object, mstring _name, JSON_value* _value)
{
    if (_object == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_mstring(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_ms_set_object(JSON_object* _object, mstring _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_array(JSON_object* _object, mstring _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_mstring(JSON_object* _object, mstring _name, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_string(JSON_object* _object, mstring _name, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_number(JSON_object* _object, mstring _name, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_number_mstring(JSON_object* _object, mstring _name, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_number_string(JSON_object* _object, mstring _name, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_boolean(JSON_object* _object, mstring _name, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_null(JSON_object* _object, mstring _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_s_set_value(JSON_object* _object, const char* _name, JSON_value* _value)
{
    return json_object_set_value(_object, _name, _value);
}

JSON_object_item* json_object_s_set_object(JSON_object* _object, const char* _name)
{
    return json_object_set_object(_object, _name);
}

JSON_object_item* json_object_s_set_array(JSON_object* _object, const char* _name)
{
    return json_object_set_array( _object, _name);
}

JSON_object_item* json_object_s_set_mstring(JSON_object* _object, const char* _name, mstring _mstring)
{
    return json_object_set_mstring(_object, _name, _mstring);
}

JSON_object_item* json_object_s_set_string(JSON_object* _object, const char* _name, const char* _string)
{
    return json_object_set_string(_object, _name, _string);
}

JSON_object_item* json_object_s_set_number(JSON_object* _object, const char* _name, double _number)
{
    return json_object_set_number(_object, _name, _number);
}

JSON_object_item* json_object_s_set_number_mstring(JSON_object* _object, const char* _name, mstring _mnumber)
{
    return json_object_set_number_mstring(_object, _name, _mnumber);
}

JSON_object_item* json_object_s_set_number_string(JSON_object* _object, const char* _name, const char* _number)
{
    return json_object_set_number_string(_object, _name, _number);
}

JSON_object_item* json_object_s_set_boolean(JSON_object* _object, const char* _name, bool _boolean)
{
    return json_object_set_boolean(_object, _name, _boolean);
}

JSON_object_item* json_object_s_set_null(JSON_object* _object, const char* _name)
{
    return json_object_set_null(_object, _name);
}


JSON_object_item* json_object_n_set_value(JSON_object* _object, double _name, JSON_value* _value)
{
    if (_object == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_number(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_n_set_object(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_array(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_mstring(JSON_object* _object, double _name, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_string(JSON_object* _object, double _name, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_number(JSON_object* _object, double _name, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_number_mstring(JSON_object* _object, double _name, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_number_string(JSON_object* _object, double _name, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_boolean(JSON_object* _object, double _name, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_null(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_nms_set_value(JSON_object* _object, mstring _name, JSON_value* _value)
{
    if (_object == NULL || _name.value == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_mstring(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_nms_set_object(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_array(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_mstring(JSON_object* _object, mstring _name, mstring _mstring)
{
    if (_object == NULL || _name.value == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_string(JSON_object* _object, mstring _name, const char* _string)
{
    if (_object == NULL || _name.value == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_number(JSON_object* _object, mstring _name, double _number)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_number_mstring(JSON_object* _object, mstring _name, mstring _mnumber)
{
    if (_object == NULL || _name.value == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_number_string(JSON_object* _object, mstring _name, const char* _number)
{
    if (_object == NULL || _name.value == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_boolean(JSON_object* _object, mstring _name, bool _boolean)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_null(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_ns_set_value(JSON_object* _object, const char* _name, JSON_value* _value)
{
    if (_object == NULL || _name == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_number_string(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_ns_set_object(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_array(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_mstring(JSON_object* _object, const char* _name, mstring _mstring)
{
    if (_object == NULL || _name == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_string(JSON_object* _object, const char* _name, const char* _string)
{
    if (_object == NULL || _name == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_number(JSON_object* _object, const char* _name, double _number)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_number_mstring(JSON_object* _object, const char* _name, mstring _mnumber)
{
    if (_object == NULL || _name == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_number_string(JSON_object* _object, const char* _name, const char* _number)
{
    if (_object == NULL || _name == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_boolean(JSON_object* _object, const char* _name, bool _boolean)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_null(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_b_set_value(JSON_object* _object, bool _name, JSON_value* _value)
{
    if (_object == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_boolean(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_b_set_object(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_array(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_mstring(JSON_object* _object, bool _name, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_string(JSON_object* _object, bool _name, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_number(JSON_object* _object, bool _name, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_number_mstring(JSON_object* _object, bool _name, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_number_string(JSON_object* _object, bool _name, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_boolean(JSON_object* _object, bool _name, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_null(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_value(_object, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_i_set_value(JSON_object* _object, JSON_value* _value)
{
    if (_object == NULL|| _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_null();
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_jname_value(_object, name, _value);
    return item;
}

JSON_object_item* json_object_i_set_object(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_array(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_mstring(JSON_object* _object, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_string(JSON_object* _object, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_number(JSON_object* _object, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_number_mstring(JSON_object* _object, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_number_string(JSON_object* _object, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_boolean(JSON_object* _object, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_null(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_value(_object, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_ms_set_ith_value(JSON_object* _object, size_t _ith, mstring _name, JSON_value* _value)
{
    if (_object == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_mstring(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_object(JSON_object* _object, size_t _ith, mstring _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_array(JSON_object* _object, size_t _ith, mstring _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_string(JSON_object* _object, size_t _ith, mstring _name, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_number(JSON_object* _object, size_t _ith, mstring _name, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_number_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_number_string(JSON_object* _object, size_t _ith, mstring _name, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_boolean(JSON_object* _object, size_t _ith, mstring _name, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ms_set_ith_null(JSON_object* _object, size_t _ith, mstring _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_s_set_ith_value(JSON_object* _object, size_t _ith, const char* _name, JSON_value* _value)
{
    return json_object_set_ith_value(_object, _ith, _name, _value);
}

JSON_object_item* json_object_s_set_ith_object(JSON_object* _object, size_t _ith, const char* _name)
{
    return json_object_set_ith_object(_object, _ith, _name);
}

JSON_object_item* json_object_s_set_ith_array(JSON_object* _object, size_t _ith, const char* _name)
{
    return json_object_set_ith_array(_object, _ith, _name);
}

JSON_object_item* json_object_s_set_ith_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mstring)
{
    return json_object_set_ith_mstring(_object, _ith, _name, _mstring);
}

JSON_object_item* json_object_s_set_ith_string(JSON_object* _object, size_t _ith, const char* _name, const char* _string)
{
    return json_object_set_ith_string(_object, _ith, _name, _string);
}

JSON_object_item* json_object_s_set_ith_number(JSON_object* _object, size_t _ith, const char* _name, double _number)
{
    return json_object_set_ith_number(_object, _ith, _name, _number);
}

JSON_object_item* json_object_s_set_ith_number_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mnumber)
{
    return json_object_set_ith_number_mstring(_object, _ith, _name, _mnumber);
}

JSON_object_item* json_object_s_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _name, const char* _number)
{
    return json_object_set_ith_number_string(_object, _ith, _name, _number);
}

JSON_object_item* json_object_s_set_ith_boolean(JSON_object* _object, size_t _ith, const char* _name, bool _boolean)
{
    return json_object_set_ith_boolean(_object, _ith, _name, _boolean);
}

JSON_object_item* json_object_s_set_ith_null(JSON_object* _object, size_t _ith, const char* _name)
{
    return json_object_set_ith_null(_object, _ith, _name);
}


JSON_object_item* json_object_n_set_ith_value(JSON_object* _object, size_t _ith, double _name, JSON_value* _value)
{
    if (_object == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_number(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_n_set_ith_object(JSON_object* _object, size_t _ith, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_array(JSON_object* _object, size_t _ith, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_mstring(JSON_object* _object, size_t _ith, double _name, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_string(JSON_object* _object, size_t _ith, double _name, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_number(JSON_object* _object, size_t _ith, double _name, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_number_mstring(JSON_object* _object, size_t _ith, double _name, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_number_string(JSON_object* _object, size_t _ith, double _name, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_boolean(JSON_object* _object, size_t _ith, double _name, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_n_set_ith_null(JSON_object* _object, size_t _ith, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_n_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_value(JSON_object* _object, size_t _ith, mstring _name, JSON_value* _value)
{
    if (_object == NULL || _name.value == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_mstring(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_object(JSON_object* _object, size_t _ith, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_array(JSON_object* _object, size_t _ith, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mstring)
{
    if (_object == NULL || _name.value == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_string(JSON_object* _object, size_t _ith, mstring _name, const char* _string)
{
    if (_object == NULL || _name.value == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_number(JSON_object* _object, size_t _ith, mstring _name, double _number)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_number_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mnumber)
{
    if (_object == NULL || _name.value == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_number_string(JSON_object* _object, size_t _ith, mstring _name, const char* _number)
{
    if (_object == NULL || _name.value == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_boolean(JSON_object* _object, size_t _ith, mstring _name, bool _boolean)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_nms_set_ith_null(JSON_object* _object, size_t _ith, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_nms_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_ns_set_ith_value(JSON_object* _object, size_t _ith, const char* _name, JSON_value* _value)
{
    if (_object == NULL || _name == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_number_string(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_object(JSON_object* _object, size_t _ith, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_array(JSON_object* _object, size_t _ith, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mstring)
{
    if (_object == NULL || _name == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_string(JSON_object* _object, size_t _ith, const char* _name, const char* _string)
{
    if (_object == NULL || _name == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_number(JSON_object* _object, size_t _ith, const char* _name, double _number)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_number_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mnumber)
{
    if (_object == NULL || _name == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _name, const char* _number)
{
    if (_object == NULL || _name == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_boolean(JSON_object* _object, size_t _ith, const char* _name, bool _boolean)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_ns_set_ith_null(JSON_object* _object, size_t _ith, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_ns_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_value(JSON_object* _object, size_t _ith, bool _name, JSON_value* _value)
{
    if (_object == NULL || _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_boolean(_name);
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_b_set_ith_object(JSON_object* _object, size_t _ith, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_array(JSON_object* _object, size_t _ith, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_mstring(JSON_object* _object, size_t _ith, bool _name, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_string(JSON_object* _object, size_t _ith, bool _name, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_number(JSON_object* _object, size_t _ith, bool _name, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_number_mstring(JSON_object* _object, size_t _ith, bool _name, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_number_string(JSON_object* _object, size_t _ith, bool _name, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_boolean(JSON_object* _object, size_t _ith, bool _name, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_b_set_ith_null(JSON_object* _object, size_t _ith, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_b_set_ith_value(_object, _ith, _name, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_object_item* json_object_i_set_ith_value(JSON_object* _object, size_t _ith, JSON_value* _value)
{
    if (_object == NULL|| _value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_name* name = json_name_create_null();
    if (name == NULL) return JSON_DEFAULT_OBJECT_ITEM;

    JSON_object_item* item = json_object_set_ith_jname_value(_object, _ith, name, _value);
    return item;
}

JSON_object_item* json_object_i_set_ith_object(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_object();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_array(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_array();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_mstring(JSON_object* _object, size_t _ith, mstring _mstring)
{
    if (_object == NULL || _mstring.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_mstring(_mstring);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_string(JSON_object* _object, size_t _ith, const char* _string)
{
    if (_object == NULL || _string == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_string(_string);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_number(JSON_object* _object, size_t _ith, double _number)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_number_mstring(JSON_object* _object, size_t _ith, mstring _mnumber)
{
    if (_object == NULL || _mnumber.value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_mstring(_mnumber);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _number)
{
    if (_object == NULL || _number == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_number_string(_number);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_boolean(JSON_object* _object, size_t _ith, bool _boolean)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_boolean(_boolean);
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}

JSON_object_item* json_object_i_set_ith_null(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_value* value = json_value_create_null();
    if (value == NULL) return JSON_DEFAULT_OBJECT_ITEM;
    JSON_object_item* item = json_object_i_set_ith_value(_object, _ith, value);
    if (item == NULL) json_value_free(value);
    return item;
}


JSON_type json_object_get_name_type(JSON_object* _object, size_t _ith)
{
    if (_object == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_get_ith_value(_object, _ith);
    return json_value_get_type(value);
}


JSON_value* json_object_ms_get_value(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_ms_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return json_object_item_get_value(item);
}

JSON_type json_object_ms_get_type(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return json_value_get_type(value);
}

JSON_object* json_object_ms_get_object(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_OBJECT;

    return json_value_get_object(value);
}

JSON_array* json_object_ms_get_array(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_ARRAY;

    return json_value_get_array(value);
}

const char* json_object_ms_get_string(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_STRING;

    return json_value_get_string(value);
}

double json_object_ms_get_number(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_number(value);
}

const char* json_object_ms_get_number_string(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER_STRING;

    return json_value_get_number_string(value);
}

double json_object_ms_get_real_number(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_real_number(value);
}

bool json_object_ms_get_boolean(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_get_boolean(value);
}

bool json_object_ms_is_null(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_ms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_is_null(value);
}


JSON_value* json_object_s_get_value(JSON_object* _object, const char* _name)
{
    return json_object_get_value(_object, _name);
}

JSON_type json_object_s_get_type(JSON_object* _object, const char* _name)
{
    return json_object_get_type(_object, _name);
}

JSON_object* json_object_s_get_object(JSON_object* _object, const char* _name)
{
    return json_object_get_object(_object, _name);
}

JSON_array* json_object_s_get_array(JSON_object* _object, const char* _name)
{
    return json_object_get_array(_object, _name);
}

const char* json_object_s_get_string(JSON_object* _object, const char* _name)
{
    return json_object_get_string(_object, _name);
}

double json_object_s_get_number(JSON_object* _object, const char* _name)
{
    return json_object_get_number(_object, _name);
}

const char* json_object_s_get_number_string(JSON_object* _object, const char* _name)
{
    return json_object_get_number_string(_object, _name);
}

double json_object_s_get_real_number(JSON_object* _object, const char* _name)
{
    return json_object_get_real_number(_object, _name);
}

bool json_object_s_get_boolean(JSON_object* _object, const char* _name)
{
    return json_object_get_boolean(_object, _name);
}

bool json_object_s_is_null(JSON_object* _object, const char* _name)
{
    return json_object_is_null(_object, _name);
}


JSON_value* json_object_n_get_value(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_n_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return json_object_item_get_value(item);
}

JSON_type json_object_n_get_type(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return json_value_get_type(value);
}

JSON_object* json_object_n_get_object(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_OBJECT;

    return json_value_get_object(value);
}

JSON_array* json_object_n_get_array(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_ARRAY;

    return json_value_get_array(value);
}

const char* json_object_n_get_string(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_STRING;

    return json_value_get_string(value);
}

double json_object_n_get_number(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_number(value);
}

const char* json_object_n_get_number_string(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER_STRING;

    return json_value_get_number_string(value);
}

double json_object_n_get_real_number(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_real_number(value);
}

bool json_object_n_get_boolean(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_get_boolean(value);
}

bool json_object_n_is_null(JSON_object* _object, double _name)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_n_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_is_null(value);
}


JSON_value* json_object_nms_get_value(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_nms_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return json_object_item_get_value(item);
}

JSON_type json_object_nms_get_type(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return json_value_get_type(value);
}

JSON_object* json_object_nms_get_object(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_OBJECT;

    return json_value_get_object(value);
}

JSON_array* json_object_nms_get_array(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_ARRAY;

    return json_value_get_array(value);
}

const char* json_object_nms_get_string(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_STRING;

    return json_value_get_string(value);
}

double json_object_nms_get_number(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_number(value);
}

const char* json_object_nms_get_number_string(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER_STRING;

    return json_value_get_number_string(value);
}

double json_object_nms_get_real_number(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_real_number(value);
}

bool json_object_nms_get_boolean(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_get_boolean(value);
}

bool json_object_nms_is_null(JSON_object* _object, mstring _name)
{
    if (_object == NULL || _name.value == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_nms_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_is_null(value);
}


JSON_value* json_object_ns_get_value(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_ns_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return json_object_item_get_value(item);
}

JSON_type json_object_ns_get_type(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_ns_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return json_value_get_type(value);
}

JSON_object* json_object_ns_get_object(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_object(value);
}

JSON_array* json_object_ns_get_array(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_array(value);
}

const char* json_object_ns_get_string(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_string(value);
}

double json_object_ns_get_number(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_number(value);
}

const char* json_object_ns_get_number_string(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_number_string(value);
}

double json_object_ns_get_real_number(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_real_number(value);
}

bool json_object_ns_get_boolean(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_boolean(value);
}

bool json_object_ns_is_null(JSON_object* _object, const char* _name)
{
    if (_object == NULL || _name == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_ns_get_value(_object, _name);

    return json_value_get_type(value) == json_type_null;
}


JSON_value* json_object_b_get_value(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_b_get_object_item(_object, _name);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return json_object_item_get_value(item);
}

JSON_type json_object_b_get_type(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return json_value_get_type(value);
}

JSON_object* json_object_b_get_object(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_OBJECT;

    return json_value_get_object(value);
}

JSON_array* json_object_b_get_array(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_ARRAY;

    return json_value_get_array(value);
}

const char* json_object_b_get_string(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_STRING;

    return json_value_get_string(value);
}

double json_object_b_get_number(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_number(value);
}

const char* json_object_b_get_number_string(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER_STRING;

    return json_value_get_number_string(value);
}

double json_object_b_get_real_number(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_real_number(value);
}

bool json_object_b_get_boolean(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_get_boolean(value);
}

bool json_object_b_is_null(JSON_object* _object, bool _name)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_b_get_value(_object, _name);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_is_null(value);
}


JSON_value* json_object_i_get_value(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_VALUE;
    JSON_object_item* item = json_object_i_get_object_item(_object);
    if (item == NULL) return JSON_DEFAULT_VALUE;

    return json_object_item_get_value(item);
}

JSON_type json_object_i_get_type(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_TYPE;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_TYPE;

    return json_value_get_type(value);
}

JSON_object* json_object_i_get_object(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_OBJECT;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_OBJECT;

    return json_value_get_object(value);
}

JSON_array* json_object_i_get_array(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_ARRAY;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_ARRAY;

    return json_value_get_array(value);
}

const char* json_object_i_get_string(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_STRING;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_STRING;

    return json_value_get_string(value);
}

double json_object_i_get_number(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_number(value);
}

const char* json_object_i_get_number_string(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER_STRING;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_NUMBER_STRING;

    return json_value_get_number_string(value);
}

double json_object_i_get_real_number(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_NUMBER;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_NUMBER;

    return json_value_get_real_number(value);
}

bool json_object_i_get_boolean(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_get_boolean(value);
}

bool json_object_i_is_null(JSON_object* _object)
{
    if (_object == NULL) return JSON_DEFAULT_BOOLEAN;
    JSON_value* value = json_object_i_get_value(_object);
    if (value == NULL) return JSON_DEFAULT_BOOLEAN;

    return json_value_is_null(value);
}
