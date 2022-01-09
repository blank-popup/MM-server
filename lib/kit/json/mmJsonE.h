#ifndef ___MM_KIT_JSONE_H___
#define ___MM_KIT_JSONE_H___

#include "mmJson.h"


#ifdef __cplusplus
extern "C" {
#endif
    JSON_object_item* json_object_ms_set_value(JSON_object* _object, mstring _name, JSON_value* _value);
    JSON_object_item* json_object_ms_set_object(JSON_object* _object, mstring _name);
    JSON_object_item* json_object_ms_set_array(JSON_object* _object, mstring _name);
    JSON_object_item* json_object_ms_set_mstring(JSON_object* _object, mstring _name, mstring _mstring);
    JSON_object_item* json_object_ms_set_string(JSON_object* _object, mstring _name, const char* _string);
    JSON_object_item* json_object_ms_set_number(JSON_object* _object, mstring _name, double _number);
    JSON_object_item* json_object_ms_set_number_mstring(JSON_object* _object, mstring _name, mstring _mnumber);
    JSON_object_item* json_object_ms_set_number_string(JSON_object* _object, mstring _name, const char* _number);
    JSON_object_item* json_object_ms_set_boolean(JSON_object* _object, mstring _name, bool _boolean);
    JSON_object_item* json_object_ms_set_null(JSON_object* _object, mstring _name);

    JSON_object_item* json_object_s_set_value(JSON_object* _object, const char* _name, JSON_value* _value);
    JSON_object_item* json_object_s_set_object(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_s_set_array(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_s_set_mstring(JSON_object* _object, const char* _name, mstring _mstring);
    JSON_object_item* json_object_s_set_string(JSON_object* _object, const char* _name, const char* _string);
    JSON_object_item* json_object_s_set_number(JSON_object* _object, const char* _name, double _number);
    JSON_object_item* json_object_s_set_number_mstring(JSON_object* _object, const char* _name, mstring _mnumber);
    JSON_object_item* json_object_s_set_number_string(JSON_object* _object, const char* _name, const char* _number);
    JSON_object_item* json_object_s_set_boolean(JSON_object* _object, const char* _name, bool _boolean);
    JSON_object_item* json_object_s_set_null(JSON_object* _object, const char* _name);

    JSON_object_item* json_object_n_set_value(JSON_object* _object, double _name, JSON_value* _value);
    JSON_object_item* json_object_n_set_object(JSON_object* _object, double _name);
    JSON_object_item* json_object_n_set_array(JSON_object* _object, double _name);
    JSON_object_item* json_object_n_set_mstring(JSON_object* _object, double _name, mstring _mstring);
    JSON_object_item* json_object_n_set_string(JSON_object* _object, double _name, const char* _string);
    JSON_object_item* json_object_n_set_number(JSON_object* _object, double _name, double _number);
    JSON_object_item* json_object_n_set_number_mstring(JSON_object* _object, double _name, mstring _mnumber);
    JSON_object_item* json_object_n_set_number_string(JSON_object* _object, double _name, const char* _number);
    JSON_object_item* json_object_n_set_boolean(JSON_object* _object, double _name, bool _boolean);
    JSON_object_item* json_object_n_set_null(JSON_object* _object, double _name);

    JSON_object_item* json_object_nms_set_value(JSON_object* _object, mstring _name, JSON_value* _value);
    JSON_object_item* json_object_nms_set_object(JSON_object* _object, mstring _name);
    JSON_object_item* json_object_nms_set_array(JSON_object* _object, mstring _name);
    JSON_object_item* json_object_nms_set_mstring(JSON_object* _object, mstring _name, mstring _mstring);
    JSON_object_item* json_object_nms_set_string(JSON_object* _object, mstring _name, const char* _string);
    JSON_object_item* json_object_nms_set_number(JSON_object* _object, mstring _name, double _number);
    JSON_object_item* json_object_nms_set_number_mstring(JSON_object* _object, mstring _name, mstring _mnumber);
    JSON_object_item* json_object_nms_set_number_string(JSON_object* _object, mstring _name, const char* _number);
    JSON_object_item* json_object_nms_set_boolean(JSON_object* _object, mstring _name, bool _boolean);
    JSON_object_item* json_object_nms_set_null(JSON_object* _object, mstring _name);

    JSON_object_item* json_object_ns_set_value(JSON_object* _object, const char* _name, JSON_value* _value);
    JSON_object_item* json_object_ns_set_object(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_ns_set_array(JSON_object* _object, const char* _name);
    JSON_object_item* json_object_ns_set_mstring(JSON_object* _object, const char* _name, mstring _mstring);
    JSON_object_item* json_object_ns_set_string(JSON_object* _object, const char* _name, const char* _string);
    JSON_object_item* json_object_ns_set_number(JSON_object* _object, const char* _name, double _number);
    JSON_object_item* json_object_ns_set_number_mstring(JSON_object* _object, const char* _name, mstring _mnumber);
    JSON_object_item* json_object_ns_set_number_string(JSON_object* _object, const char* _name, const char* _number);
    JSON_object_item* json_object_ns_set_boolean(JSON_object* _object, const char* _name, bool _boolean);
    JSON_object_item* json_object_ns_set_null(JSON_object* _object, const char* _name);

    JSON_object_item* json_object_b_set_value(JSON_object* _object, bool _name, JSON_value* _value);
    JSON_object_item* json_object_b_set_object(JSON_object* _object, bool _name);
    JSON_object_item* json_object_b_set_array(JSON_object* _object, bool _name);
    JSON_object_item* json_object_b_set_mstring(JSON_object* _object, bool _name, mstring _mstring);
    JSON_object_item* json_object_b_set_string(JSON_object* _object, bool _name, const char* _string);
    JSON_object_item* json_object_b_set_number(JSON_object* _object, bool _name, double _number);
    JSON_object_item* json_object_b_set_number_mstring(JSON_object* _object, bool _name, mstring _mnumber);
    JSON_object_item* json_object_b_set_number_string(JSON_object* _object, bool _name, const char* _number);
    JSON_object_item* json_object_b_set_boolean(JSON_object* _object, bool _name, bool _boolean);
    JSON_object_item* json_object_b_set_null(JSON_object* _object, bool _name);

    JSON_object_item* json_object_i_set_value(JSON_object* _object, JSON_value* _value);
    JSON_object_item* json_object_i_set_object(JSON_object* _object);
    JSON_object_item* json_object_i_set_array(JSON_object* _object);
    JSON_object_item* json_object_i_set_mstring(JSON_object* _object, mstring _mstring);
    JSON_object_item* json_object_i_set_string(JSON_object* _object, const char* _string);
    JSON_object_item* json_object_i_set_number(JSON_object* _object, double _number);
    JSON_object_item* json_object_i_set_number_mstring(JSON_object* _object, mstring _mnumber);
    JSON_object_item* json_object_i_set_number_string(JSON_object* _object, const char* _number);
    JSON_object_item* json_object_i_set_boolean(JSON_object* _object, bool _boolean);
    JSON_object_item* json_object_i_set_null(JSON_object* _object);

    JSON_object_item* json_object_ms_set_ith_value(JSON_object* _object, size_t _ith, mstring _name, JSON_value* _value);
    JSON_object_item* json_object_ms_set_ith_object(JSON_object* _object, size_t _ith, mstring _name);
    JSON_object_item* json_object_ms_set_ith_array(JSON_object* _object, size_t _ith, mstring _name);
    JSON_object_item* json_object_ms_set_ith_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mstring);
    JSON_object_item* json_object_ms_set_ith_string(JSON_object* _object, size_t _ith, mstring _name, const char* _string);
    JSON_object_item* json_object_ms_set_ith_number(JSON_object* _object, size_t _ith, mstring _name, double _number);
    JSON_object_item* json_object_ms_set_ith_number_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mnumber);
    JSON_object_item* json_object_ms_set_ith_number_string(JSON_object* _object, size_t _ith, mstring _name, const char* _number);
    JSON_object_item* json_object_ms_set_ith_boolean(JSON_object* _object, size_t _ith, mstring _name, bool _boolean);
    JSON_object_item* json_object_ms_set_ith_null(JSON_object* _object, size_t _ith, mstring _name);

    JSON_object_item* json_object_s_set_ith_value(JSON_object* _object, size_t _ith, const char* _name, JSON_value* _value);
    JSON_object_item* json_object_s_set_ith_object(JSON_object* _object, size_t _ith, const char* _name);
    JSON_object_item* json_object_s_set_ith_array(JSON_object* _object, size_t _ith, const char* _name);
    JSON_object_item* json_object_s_set_ith_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mstring);
    JSON_object_item* json_object_s_set_ith_string(JSON_object* _object, size_t _ith, const char* _name, const char* _string);
    JSON_object_item* json_object_s_set_ith_number(JSON_object* _object, size_t _ith, const char* _name, double _number);
    JSON_object_item* json_object_s_set_ith_number_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mnumber);
    JSON_object_item* json_object_s_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _name, const char* _number);
    JSON_object_item* json_object_s_set_ith_boolean(JSON_object* _object, size_t _ith, const char* _name, bool _boolean);
    JSON_object_item* json_object_s_set_ith_null(JSON_object* _object, size_t _ith, const char* _name);

    JSON_object_item* json_object_n_set_ith_value(JSON_object* _object, size_t _ith, double _name, JSON_value* _value);
    JSON_object_item* json_object_n_set_ith_object(JSON_object* _object, size_t _ith, double _name);
    JSON_object_item* json_object_n_set_ith_array(JSON_object* _object, size_t _ith, double _name);
    JSON_object_item* json_object_n_set_ith_mstring(JSON_object* _object, size_t _ith, double _name, mstring _mstring);
    JSON_object_item* json_object_n_set_ith_string(JSON_object* _object, size_t _ith, double _name, const char* _string);
    JSON_object_item* json_object_n_set_ith_number(JSON_object* _object, size_t _ith, double _name, double _number);
    JSON_object_item* json_object_n_set_ith_number_mstring(JSON_object* _object, size_t _ith, double _name, mstring _mnumber);
    JSON_object_item* json_object_n_set_ith_number_string(JSON_object* _object, size_t _ith, double _name, const char* _number);
    JSON_object_item* json_object_n_set_ith_boolean(JSON_object* _object, size_t _ith, double _name, bool _boolean);
    JSON_object_item* json_object_n_set_ith_null(JSON_object* _object, size_t _ith, double _name);

    JSON_object_item* json_object_nms_set_ith_value(JSON_object* _object, size_t _ith, mstring _name, JSON_value* _value);
    JSON_object_item* json_object_nms_set_ith_object(JSON_object* _object, size_t _ith, mstring _name);
    JSON_object_item* json_object_nms_set_ith_array(JSON_object* _object, size_t _ith, mstring _name);
    JSON_object_item* json_object_nms_set_ith_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mstring);
    JSON_object_item* json_object_nms_set_ith_string(JSON_object* _object, size_t _ith, mstring _name, const char* _string);
    JSON_object_item* json_object_nms_set_ith_number(JSON_object* _object, size_t _ith, mstring _name, double _number);
    JSON_object_item* json_object_nms_set_ith_number_mstring(JSON_object* _object, size_t _ith, mstring _name, mstring _mnumber);
    JSON_object_item* json_object_nms_set_ith_number_string(JSON_object* _object, size_t _ith, mstring _name, const char* _number);
    JSON_object_item* json_object_nms_set_ith_boolean(JSON_object* _object, size_t _ith, mstring _name, bool _boolean);
    JSON_object_item* json_object_nms_set_ith_null(JSON_object* _object, size_t _ith, mstring _name);

    JSON_object_item* json_object_ns_set_ith_value(JSON_object* _object, size_t _ith, const char* _name, JSON_value* _value);
    JSON_object_item* json_object_ns_set_ith_object(JSON_object* _object, size_t _ith, const char* _name);
    JSON_object_item* json_object_ns_set_ith_array(JSON_object* _object, size_t _ith, const char* _name);
    JSON_object_item* json_object_ns_set_ith_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mstring);
    JSON_object_item* json_object_ns_set_ith_string(JSON_object* _object, size_t _ith, const char* _name, const char* _string);
    JSON_object_item* json_object_ns_set_ith_number(JSON_object* _object, size_t _ith, const char* _name, double _number);
    JSON_object_item* json_object_ns_set_ith_number_mstring(JSON_object* _object, size_t _ith, const char* _name, mstring _mnumber);
    JSON_object_item* json_object_ns_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _name, const char* _number);
    JSON_object_item* json_object_ns_set_ith_boolean(JSON_object* _object, size_t _ith, const char* _name, bool _boolean);
    JSON_object_item* json_object_ns_set_ith_null(JSON_object* _object, size_t _ith, const char* _name);

    JSON_object_item* json_object_b_set_ith_value(JSON_object* _object, size_t _ith, bool _name, JSON_value* _value);
    JSON_object_item* json_object_b_set_ith_object(JSON_object* _object, size_t _ith, bool _name);
    JSON_object_item* json_object_b_set_ith_array(JSON_object* _object, size_t _ith, bool _name);
    JSON_object_item* json_object_b_set_ith_mstring(JSON_object* _object, size_t _ith, bool _name, mstring _mstring);
    JSON_object_item* json_object_b_set_ith_string(JSON_object* _object, size_t _ith, bool _name, const char* _string);
    JSON_object_item* json_object_b_set_ith_number(JSON_object* _object, size_t _ith, bool _name, double _number);
    JSON_object_item* json_object_b_set_ith_number_mstring(JSON_object* _object, size_t _ith, bool _name, mstring _mnumber);
    JSON_object_item* json_object_b_set_ith_number_string(JSON_object* _object, size_t _ith, bool _name, const char* _number);
    JSON_object_item* json_object_b_set_ith_boolean(JSON_object* _object, size_t _ith, bool _name, bool _boolean);
    JSON_object_item* json_object_b_set_ith_null(JSON_object* _object, size_t _ith, bool _name);

    JSON_object_item* json_object_i_set_ith_value(JSON_object* _object, size_t _ith, JSON_value* _value);
    JSON_object_item* json_object_i_set_ith_object(JSON_object* _object, size_t _ith);
    JSON_object_item* json_object_i_set_ith_array(JSON_object* _object, size_t _ith);
    JSON_object_item* json_object_i_set_ith_mstring(JSON_object* _object, size_t _ith, mstring _mstring);
    JSON_object_item* json_object_i_set_ith_string(JSON_object* _object, size_t _ith, const char* _string);
    JSON_object_item* json_object_i_set_ith_number(JSON_object* _object, size_t _ith, double _number);
    JSON_object_item* json_object_i_set_ith_number_mstring(JSON_object* _object, size_t _ith, mstring _mnumber);
    JSON_object_item* json_object_i_set_ith_number_string(JSON_object* _object, size_t _ith, const char* _number);
    JSON_object_item* json_object_i_set_ith_boolean(JSON_object* _object, size_t _ith, bool _boolean);
    JSON_object_item* json_object_i_set_ith_null(JSON_object* _object, size_t _ith);

    JSON_type json_object_get_name_type(JSON_object* _object, size_t _ith);

    JSON_value* json_object_ms_get_value(JSON_object* _object, mstring _name);
    JSON_type json_object_ms_get_type(JSON_object* _object, mstring _name);
    JSON_object* json_object_ms_get_object(JSON_object* _object, mstring _name);
    JSON_array* json_object_ms_get_array(JSON_object* _object, mstring _name);
    const char* json_object_ms_get_string(JSON_object* _object, mstring _name);
    double json_object_ms_get_number(JSON_object* _object, mstring _name);
    const char* json_object_ms_get_number_string(JSON_object* _object, mstring _name);
    double json_object_ms_get_real_number(JSON_object* _object, mstring _name);
    bool json_object_ms_get_boolean(JSON_object* _object, mstring _name);
    bool json_object_ms_is_null(JSON_object* _object, mstring _name);

    JSON_value* json_object_s_get_value(JSON_object* _object, const char* _name);
    JSON_type json_object_s_get_type(JSON_object* _object, const char* _name);
    JSON_object* json_object_s_get_object(JSON_object* _object, const char* _name);
    JSON_array* json_object_s_get_array(JSON_object* _object, const char* _name);
    const char* json_object_s_get_string(JSON_object* _object, const char* _name);
    double json_object_s_get_number(JSON_object* _object, const char* _name);
    const char* json_object_s_get_number_string(JSON_object* _object, const char* _name);
    double json_object_s_get_real_number(JSON_object* _object, const char* _name);
    bool json_object_s_get_boolean(JSON_object* _object, const char* _name);
    bool json_object_s_is_null(JSON_object* _object, const char* _name);

    JSON_value* json_object_n_get_value(JSON_object* _object, double _name);
    JSON_type json_object_n_get_type(JSON_object* _object, double _name);
    JSON_object* json_object_n_get_object(JSON_object* _object, double _name);
    JSON_array* json_object_n_get_array(JSON_object* _object, double _name);
    const char* json_object_n_get_string(JSON_object* _object, double _name);
    double json_object_n_get_number(JSON_object* _object, double _name);
    const char* json_object_n_get_number_string(JSON_object* _object, double _name);
    double json_object_n_get_real_number(JSON_object* _object, double _name);
    bool json_object_n_get_boolean(JSON_object* _object, double _name);
    bool json_object_n_is_null(JSON_object* _object, double _name);

    JSON_value* json_object_nms_get_value(JSON_object* _object, mstring _name);
    JSON_type json_object_nms_get_type(JSON_object* _object, mstring _name);
    JSON_object* json_object_nms_get_object(JSON_object* _object, mstring _name);
    JSON_array* json_object_nms_get_array(JSON_object* _object, mstring _name);
    const char* json_object_nms_get_string(JSON_object* _object, mstring _name);
    double json_object_nms_get_number(JSON_object* _object, mstring _name);
    const char* json_object_nms_get_number_string(JSON_object* _object, mstring _name);
    double json_object_nms_get_real_number(JSON_object* _object, mstring _name);
    bool json_object_nms_get_boolean(JSON_object* _object, mstring _name);
    bool json_object_nms_is_null(JSON_object* _object, mstring _name);

    JSON_value* json_object_ns_get_value(JSON_object* _object, const char* _name);
    JSON_type json_object_ns_get_type(JSON_object* _object, const char* _name);
    JSON_object* json_object_ns_get_object(JSON_object* _object, const char* _name);
    JSON_array* json_object_ns_get_array(JSON_object* _object, const char* _name);
    const char* json_object_ns_get_string(JSON_object* _object, const char* _name);
    double json_object_ns_get_number(JSON_object* _object, const char* _name);
    const char* json_object_ns_get_number_string(JSON_object* _object, const char* _name);
    double json_object_ns_get_real_number(JSON_object* _object, const char* _name);
    bool json_object_ns_get_boolean(JSON_object* _object, const char* _name);
    bool json_object_ns_is_null(JSON_object* _object, const char* _name);

    JSON_value* json_object_b_get_value(JSON_object* _object, bool _name);
    JSON_type json_object_b_get_type(JSON_object* _object, bool _name);
    JSON_object* json_object_b_get_object(JSON_object* _object, bool _name);
    JSON_array* json_object_b_get_array(JSON_object* _object, bool _name);
    const char* json_object_b_get_string(JSON_object* _object, bool _name);
    double json_object_b_get_number(JSON_object* _object, bool _name);
    const char* json_object_b_get_number_string(JSON_object* _object, bool _name);
    double json_object_b_get_real_number(JSON_object* _object, bool _name);
    bool json_object_b_get_boolean(JSON_object* _object, bool _name);
    bool json_object_b_is_null(JSON_object* _object, bool _name);

    JSON_value* json_object_i_get_value(JSON_object* _object);
    JSON_type json_object_i_get_type(JSON_object* _object);
    JSON_object* json_object_i_get_object(JSON_object* _object);
    JSON_array* json_object_i_get_array(JSON_object* _object);
    const char* json_object_i_get_string(JSON_object* _object);
    double json_object_i_get_number(JSON_object* _object);
    const char* json_object_i_get_number_string(JSON_object* _object);
    double json_object_i_get_real_number(JSON_object* _object);
    bool json_object_i_get_boolean(JSON_object* _object);
    bool json_object_i_is_null(JSON_object* _object);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_JSONE_H___
