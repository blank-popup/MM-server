#include "restExt.h"


rcode rest_blocking_recv_all(c8** _data, size_t* _size, i32 _fd)
{
    c8* position_delimiter = NULL;
    size_t size_buffer = SOCKET_SIZE_RECV_BUFFER;
    c8* buffer = memloc(size_buffer * sizeof(char));

    while (position_delimiter == NULL) {
        memset(buffer, 0x00, size_buffer);
        i32 nread = recv(_fd, buffer, size_buffer - 1, 0);

        if (*_data == NULL) {
            *_size = nread + 1;
            *_data = memloc(*_size * sizeof(char));
        }
        else {
            *_size = strlen(*_data) + strlen(buffer) + 1;
            memory_reallocate((void**)_data, strlen(*_data) + 1, *_size);
        }

        memcpy(*_data + strlen(*_data), buffer, nread);

        position_delimiter = strstr(*_data, REST_DELIMITER_REST_HEAD_AND_BODY);
    }

    size_t length_head = position_delimiter - *_data;

    c8* position_content_length_name = string_stristr(*_data, REST_NAME_CONTENT_LENGTH);
    if (position_content_length_name == NULL) {
        size_t size_body_req = 0;
        *_size = length_head + strlen(REST_DELIMITER_REST_HEAD_AND_BODY) + size_body_req + 1;
        *_data = (c8*)memory_reallocate((void*)*_data, strlen(*_data) + 1, *_size);
    }
    else {
        c8* position_content_length_colon = strchr(position_content_length_name, ':');
        c8* position_content_length_value = position_content_length_colon + 1;
        while (*position_content_length_value == ' ') {
            ++position_content_length_value;
        }
        size_t size_body_req = (size_t)strtol(position_content_length_value, NULL, 10);
        *_size = length_head + strlen(REST_DELIMITER_REST_HEAD_AND_BODY) + size_body_req + 1;
        *_data = (c8*)memory_reallocate((void*)*_data, strlen(*_data) + 1, *_size);
        while (strlen(*_data) + 1 < *_size) {
            memset(buffer, 0x00, size_buffer);
            i32 nread = recv(_fd, buffer, size_buffer - 1, 0);
            memcpy(*_data + strlen(*_data), buffer, nread);
        }
    }

    memfree(buffer);

    return mma(ack);
}

rcode rest_blocking_send_all(c8* _data, size_t _length, i32 _fd)
{
    i32 nsend = send(_fd, _data, _length, 0);
    if (nsend < 0) {
        return mma(nak);
    }

    return mma(ack);
}


const c8* convert_version_to_scheme(i32 _version)
{
    if (_version == 11) {
        return REST_SCHEME_HTTP_1_1;
    }
    if (_version == 20) {
        return REST_SCHEME_HTTP_2;
    }

    return NULL;
}

i32 convert_scheme_to_version(const c8* _scheme)
{
    if (_scheme == NULL) {
        return -1;
    }
    if (strcmp(_scheme, REST_SCHEME_HTTP_1_1) == 0) {
        return 11;
    }
    if (strcmp(_scheme, REST_SCHEME_HTTP_2) == 0) {
        return 20;
    }

    return -1;
}


c8* rest_path_convert_array_to_string(JSON_array* _path)
{
    if (_path == NULL) {
        return NULL;
    }

    JSON_array_item* item = json_array_get_item_head(_path);
    size_t length = 1;
    while (item != NULL) {
        if (item == json_array_get_item_head(_path)) {
            length += strlen(json_value_get_string(json_array_item_get_value(item)));
        }
        else {
            length += 1 + strlen(json_value_get_string(json_array_item_get_value(item)));
        }
        item = json_array_item_get_next(item);
    }
    c8* path = memloc(length + 1);
    if (path == NULL) {
        return NULL;
    }
    string_memset_print(path, length + 1, "%s", "/");
    item = json_array_get_item_head(_path);
    while (item != NULL) {
        if (item == json_array_get_item_head(_path)) {
            string_memset_print(path + strlen(path), length + 1 - strlen(path), "%s", json_value_get_string(json_array_item_get_value(item)));
        }
        else {
            string_memset_print(path + strlen(path), length + 1 - strlen(path), "/%s", json_value_get_string(json_array_item_get_value(item)));
        }
        item = json_array_item_get_next(item);
    }

    return path;
}

JSON_value* rest_path_convert_string_to_array(c8* _path, size_t _length)
{
    if (_path == NULL || _path[0] != '/' || _length == 0 || _path[_length - 1] == '/' || strstr(_path, "//") != NULL) {
        return NULL;
    }

    JSON_value* value = json_value_create_array();
    JSON_array* array = json_value_get_array(value);

    if (_path[0] == '/' && _length == 1) {
        json_array_append_string(array, "");
        return value;
    }

    c8* pointer = strtok(&_path[1], "/");
    while (pointer != NULL) {
        json_array_append_string(array, pointer);
        strtok(NULL, "/");
    }

    return value;
}


bool rest_method_is_valid(mstring _method)
{
    if (_method.length == strlen(REST_METHOD_OPTIONS) && strncmp(_method.value, REST_METHOD_OPTIONS, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_GET) && strncmp(_method.value, REST_METHOD_GET, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_HEAD) && strncmp(_method.value, REST_METHOD_HEAD, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_POST) && strncmp(_method.value, REST_METHOD_POST, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_PUT) && strncmp(_method.value, REST_METHOD_PUT, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_DELETE) && strncmp(_method.value, REST_METHOD_DELETE, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_TRACE) && strncmp(_method.value, REST_METHOD_TRACE, _method.length) == 0) {
        return true;
    }
    else if (_method.length == strlen(REST_METHOD_CONNECT) && strncmp(_method.value, REST_METHOD_CONNECT, _method.length) == 0) {
        return true;
    }

    return false;
}

bool rest_req_line_is_valid(JSON_value* _value_req)
{
    if (_value_req == NULL) return false;

    JSON_object* object_req = json_value_get_object(_value_req);
    if (object_req == NULL) return false;
    JSON_object* line = json_object_get_object(object_req, REST_NAME_LINE);
    if (line == NULL) return false;
    const c8* method = json_object_get_string(line, REST_NAME_METHOD);
    if (method == NULL || strlen(method) == 0) return false;
    JSON_array* path = json_object_get_array(line, REST_NAME_PATH);
    if (path == NULL || json_array_get_length(path) == 0) return false;
    const c8* scheme = json_object_get_string(line, REST_NAME_SCHEME);
    if (scheme == NULL || strlen(scheme) == 0) return false;

    return true;
}

bool rest_res_line_is_valid(JSON_value* _value_res)
{
    if (_value_res == NULL) return false;

    JSON_object* object_res = json_value_get_object(_value_res);
    if (object_res == NULL) return false;
    JSON_object* line = json_object_get_object(object_res, REST_NAME_LINE);
    if (line == NULL) return false;
    const c8* scheme = json_object_get_string(line, REST_NAME_SCHEME);
    if (scheme == NULL || strlen(scheme) == 0) return false;
    if (json_object_has_name(line, REST_NAME_CODE) == false) return false;
    const c8* message = json_object_get_string(line, REST_NAME_MESSAGE);
    if (message == NULL || strlen(message) == 0) return false;

    return true;
}


static c8* rest_find_character(c8* _content, size_t _length, c8 _character)
{
    if (_content == NULL) {
        return NULL;
    }

    c8* character = _content;
    for (size_t ii = 0; ii < _length; ++ii) {
        if (*(character + ii) == _character) {
            return character + ii;
        }
    }

    return NULL;
}

static c8* rest_find_non_character(c8* _content, size_t _length, c8 _character)
{
    if (_content == NULL) {
        return NULL;
    }

    c8* character = _content;
    for (size_t ii = 0; ii < _length; ++ii) {
        if (*(character + ii) != _character) {
            return character + ii;
        }
    }

    return NULL;
}

static bool rest_content_has_no_space(c8* _content, size_t _length)
{
    if (_content == NULL) {
        return false;
    }

    c8* character = _content;
    for (size_t ii = 0; ii < _length; ++ii) {
        if (isspace(*(character + ii)) != 0) {
            return false;
        }
    }

    return true;
}


static rcode rest_set_line_path(JSON_array* _array_path, c8* _start, c8* _finish)
{
    if (_array_path == NULL || _start == NULL || _finish == NULL) {
        return mma(nak);
    }

    mstring jpath_partial = { 0 };
    jpath_partial.value = _start;
    jpath_partial.length = _finish - _start;
    if (json_array_append_mstring(_array_path, jpath_partial) == NULL) {
        return mma(nak);
    }

    return mma(ack);
}

static rcode rest_set_line_path_query(JSON_object* _object_query, c8* _start, c8* _finish)
{
    if (_object_query == NULL || _start == NULL || _finish == NULL) {
        return mma(nak);
    }

    mstring jnamevalue;
    jnamevalue.value = _start;
    jnamevalue.length = _finish - _start;
    c8* equal = rest_find_character(jnamevalue.value, jnamevalue.length, REST_CHARACTER_EQUAL);
    if (equal == NULL) {
        lerror(l_app, "Invalid query");
        return mma(nak);
    }
    mstring jname;
    jname.value = _start;
    jname.length = equal - _start;
    mstring jvalue;
    jvalue.value = equal + 1;
    jvalue.length = _finish - equal - 1;
    JSON_name* name_jname = json_name_create_mstring(jname);
    JSON_value* value_jvalue = json_value_create_mstring(jvalue);
    if (json_object_set_jname_value(_object_query, name_jname, value_jvalue) == NULL) {
        return mma(nak);
    }

    return mma(ack);
}

rcode rest_set_value_req_line(JSON_value** _value_line, c8* _sock_req)
{
    if (_value_line == NULL || *_value_line != NULL || _sock_req == NULL) {
        return mma(nak);
    }

    c8* position_return = rest_find_character(_sock_req, strlen(_sock_req), REST_CHARACTER_RETURN);
    if (position_return == NULL) {
        lerror(l_app, "Invalid request");
        return mma(nak);
    }

    *_value_line = json_value_create_object();
    JSON_object* object_line = json_value_get_object(*_value_line);

    c8* position_method_start = rest_find_non_character(_sock_req, position_return - _sock_req, REST_CHARACTER_SPACE);
    c8* position_space_first = rest_find_character(position_method_start + 1, position_return - position_method_start - 1, REST_CHARACTER_SPACE);
    if (position_space_first == NULL) {
        lerror(l_app, "Fail to find first space");
        return mma(nak);
    }
    mstring method;
    method.value = position_method_start;
    method.length =position_space_first - position_method_start;
    // if (rest_method_is_valid(method) == false) {
    //     return mma(nak);
    // }
    json_object_set_mstring(object_line, REST_NAME_METHOD, method);

    c8* position_path_start = rest_find_non_character(position_space_first + 1, position_return - position_space_first - 1, REST_CHARACTER_SPACE);
    c8* position_space_second = rest_find_character(position_path_start + 1, position_return - position_path_start - 1, REST_CHARACTER_SPACE);
    c8* position_path_question = rest_find_character(position_path_start + 1, position_return - position_path_start - 1, REST_CHARACTER_QUESTION);
    if (position_space_second == NULL) {
        lerror(l_app, "Fail to find second space");
        return mma(nak);
    }
    mstring mpath;
    mpath.value = position_path_start;
    mpath.length = position_space_second - position_path_start;
    mstring mpath_without_query;
    if (position_path_question != NULL && position_path_question < position_space_second) {
        mpath_without_query.value = mpath.value;
        mpath_without_query.length = position_path_question - position_path_start;
    }
    else {
        mpath_without_query.value = mpath.value;
        mpath_without_query.length = mpath.length;
    }
    if (mpath_without_query.value[0] != '/') {
        lerror(l_app, "Invalid path not starting with \"/\"");
        return mma(nak);
    }
    if (mpath_without_query.length > 1 && mpath_without_query.value[mpath_without_query.length - 1] == '/') {
        lerror(l_app, "Invalid path finishing with \"/\"");
        return mma(nak);
    }
    mstring slash_double = mstring_strstr(mpath_without_query, (mstring){ "//", strlen("//")});
    if (slash_double.value != NULL || slash_double.value != 0) {
        lerror(l_app, "Invalid path including \"//\"");
        return mma(nak);
    }
    json_object_set_array(object_line, REST_NAME_PATH);
    JSON_array* array_path = json_object_get_array(object_line, REST_NAME_PATH);
    if (mpath_without_query.value[0] == '/' && mpath_without_query.length == 1) {
        json_array_append_string(array_path, "");
    }
    else {
        c8* position_path_partial = mpath_without_query.value + 1;
        size_t length_path_partial_remain = mpath_without_query.length - 1;
        while (true) {
            c8* position_slash = rest_find_character(position_path_partial, length_path_partial_remain, REST_CHARACTER_SLASH);
            if (position_slash == NULL) {
                if (rest_set_line_path(array_path, position_path_partial, mpath_without_query.value + mpath_without_query.length) < ack) {
                    return mma(nak);
                }
                break;
            } else {
                if (rest_set_line_path(array_path, position_path_partial, position_slash) < ack) {
                    return mma(nak);
                }
            }
            length_path_partial_remain -= position_slash - position_path_partial;
            position_path_partial = position_slash + 1;
        }
    }

    json_object_set_object(object_line, REST_NAME_QUERY);
    JSON_object* object_query = json_object_get_object(object_line, REST_NAME_QUERY);
    if (position_path_question != NULL) {
        c8* position_namevalue = position_path_question + 1;
        while (true) {
            c8* position_ampersand = rest_find_character(position_namevalue, position_space_second - position_namevalue, REST_CHARACTER_AMPERSAND);
            if (position_ampersand == NULL) {
                if (rest_set_line_path_query(object_query, position_namevalue, position_space_second) < ack) {
                    return mma(nak);
                }
                break;
            } else {
                if (rest_set_line_path_query(object_query, position_namevalue, position_ampersand) < ack) {
                    return mma(nak);
                }
            }
            position_namevalue = position_ampersand + 1;
        }
    }

    c8* position_scheme_start = rest_find_non_character(position_space_second + 1, position_return - position_space_second - 1, REST_CHARACTER_SPACE);
    mstring jscheme;
    jscheme.value = position_scheme_start;
    jscheme.length = position_return - position_scheme_start;
    if (rest_content_has_no_space(jscheme.value, jscheme.length) == false) {
        lerror(l_app, "Invalid scheme");
        return mma(nak);
    }
    json_object_set_mstring(object_line, REST_NAME_SCHEME, jscheme);

    return mma(ack);
}

rcode rest_set_value_req_head(JSON_value** _value_head, c8* _sock_req)
{
    if (_value_head == NULL || *_value_head != NULL || _sock_req == NULL) {
        return mma(nak);
    }

    *_value_head = json_value_create_object();
    JSON_object* object_head = json_value_get_object(*_value_head);

    c8* position_start = strstr(_sock_req, REST_DELIMITER_REST_HEAD);
    if (position_start == NULL) return mma(nak);
    c8* position_end = strstr(_sock_req, REST_DELIMITER_REST_HEAD_AND_BODY);
    if (position_end == NULL) return mma(nak);
    c8* position = position_start;

    while (position != NULL && position < position_end) {
        c8* position_colon = strchr(position + 1, REST_CHARACTER_COLON);
        if (position_colon == NULL) return mma(nak);
        c8* position_line_end = strstr(position + 1, REST_DELIMITER_REST_HEAD);
        if (position_line_end == NULL) return mma(nak);
        c8* position_name_start = position;
        c8* position_name_end = position_colon;
        c8* position_value_start = position_colon + 1;
        c8* position_value_end = position_line_end;
        while (isspace(*position_name_start) != 0) ++position_name_start;
        while (isspace(*(position_name_end - 1)) != 0) --position_name_end;
        while (isspace(*position_value_start) != 0) ++position_value_start;
        while (isspace(*(position_value_end - 1)) != 0) --position_value_end;
        c8* name = memloc(position_name_end - position_name_start + 1);
        if (name == NULL) return mma(nak);
        strncpy(name, position_name_start, position_name_end - position_name_start);
        name[position_name_end - position_name_start] = REST_CHARACTER_NULL;
        json_object_set_mstring(object_head, name, (mstring){ position_value_start, position_value_end - position_value_start });
        memfree(name);

        position = position_line_end;
    }

    return mma(ack);
}

static bool rest_has_body(c8* _sock_req)
{
    if (_sock_req == NULL) {
        return false;
    }

    if (strlen(_sock_req) < 4) {
        return false;
    }
    c8* string_last_four = _sock_req + strlen(_sock_req) - 4;
    if (strcmp(string_last_four, REST_DELIMITER_REST_HEAD_AND_BODY) == 0) {
        return false;
    }

    return true;
}

rcode rest_set_value_req_body(JSON_value** _value_body, c8* _sock_req)
{
    if (_value_body == NULL || *_value_body != NULL || _sock_req == NULL) {
        return mma(nak);
    }
    if (rest_has_body(_sock_req) == false) {
        return mma(ack);
    }

    c8* position_header_end = strstr(_sock_req, REST_DELIMITER_REST_HEAD_AND_BODY);
    if (position_header_end == NULL) return mma(nak);

    c8* position_body_start = position_header_end;
    while (isspace(*position_body_start) != 0) ++position_body_start;

    *_value_body = json_parse_content(position_body_start);
    if (_value_body == NULL) {
        return mma(nak);
    }

    return mma(ack);
}


rcode rest_set_value_res_line(JSON_value* _value_res, code_response _code, const c8* _scheme)
{
    if (_value_res == NULL || _scheme == NULL) {
        return mma(nak);
    }

    JSON_object* object = json_value_get_object(_value_res);
    if (json_object_has_name(object, REST_NAME_LINE) == false) {
        json_object_set_object(object, REST_NAME_LINE);
    }
    JSON_object* line = json_object_get_object(object, REST_NAME_LINE);
    if (line == NULL) {
        return mma(nak);
    }
    json_object_set_string(line, REST_NAME_SCHEME, _scheme);
    json_object_set_number(line, REST_NAME_CODE, (double)(int)_code);

    if (_code == REST_RESPONSE_CODE_200) {
        json_object_set_string(line, REST_NAME_MESSAGE, REST_RESPONSE_LINE_MESSAGE_200);
    }
    else if (_code == REST_RESPONSE_CODE_201) {
        json_object_set_string(line, REST_NAME_MESSAGE, REST_RESPONSE_LINE_MESSAGE_201);
    }
    else if (_code == REST_RESPONSE_CODE_204) {
        json_object_set_string(line, REST_NAME_MESSAGE, REST_RESPONSE_LINE_MESSAGE_204);
    }
    else if (_code == REST_RESPONSE_CODE_404) {
        json_object_set_string(line, REST_NAME_MESSAGE, REST_RESPONSE_LINE_MESSAGE_404);
    }
    else if (_code == REST_RESPONSE_CODE_500) {
        json_object_set_string(line, REST_NAME_MESSAGE, REST_RESPONSE_LINE_MESSAGE_500);
    }

    return mma(ack);
}

rcode rest_set_value_res_head(JSON_value* _value_res, const c8* _name, const c8* _value)
{
    if (_value_res == NULL || _name == NULL || _value == NULL) {
        return mma(nak);
    }

    JSON_object* object = json_value_get_object(_value_res);
    if (json_object_has_name(object, REST_NAME_HEAD) == false) {
        json_object_set_object(object, REST_NAME_HEAD);
    }
    JSON_object* head = json_object_get_object(object, REST_NAME_HEAD);
    if (head == NULL) {
        return mma(nak);
    }

    json_object_set_string(head, _name, _value);

    return mma(ack);
}

rcode rest_set_value_res_body_string(JSON_value* _value_res, const c8* _name, const c8* _value)
{
    if (_value_res == NULL || _name == NULL || _value == NULL) {
        return mma(nak);
    }

    JSON_object* object = json_value_get_object(_value_res);
    if (json_object_has_name(object, REST_NAME_BODY) == false) {
        json_object_set_object(object, REST_NAME_BODY);
    }
    JSON_object* body = json_object_get_object(object, REST_NAME_BODY);
    if (body == NULL) {
        return mma(nak);
    }

    json_object_set_string(body, _name, _value);

    return mma(ack);
}

rcode rest_set_value_res_body_number(JSON_value* _value_res, const c8* _name, double _value)
{
    if (_value_res == NULL || _name == NULL) {
        return mma(nak);
    }

    JSON_object* object = json_value_get_object(_value_res);
    if (json_object_has_name(object, REST_NAME_BODY) == false) {
        json_object_set_object(object, REST_NAME_BODY);
    }
    JSON_object* body = json_object_get_object(object, REST_NAME_BODY);
    if (body == NULL) {
        return mma(nak);
    }

    json_object_set_number(body, _name, _value);

    return mma(ack);
}


rcode rest_get_length_res_line(size_t* _length, JSON_value* _value_res)
{
    if (_length == NULL || _value_res == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_value_res);
    JSON_object* line = json_object_get_object(object_res, REST_NAME_LINE);

    const c8* scheme = json_object_get_string(line, REST_NAME_SCHEME);
    if (scheme == NULL) {
        return mma(nak);
    }
    if (json_object_has_name(line, REST_NAME_CODE) == false) {
        return mma(nak);
    }
    const c8* message = json_object_get_string(line, REST_NAME_MESSAGE);
    if (message == NULL) {
        return mma(nak);
    }

    *_length = strlen(scheme) + strlen(REST_DELIMITER_SPACE) + strlen("000") + strlen(REST_DELIMITER_SPACE) + strlen(message) + strlen(REST_DELIMITER_REST_HEAD);

    return mma(ack);
}

rcode rest_get_length_res_head(size_t* _length, JSON_value* _value_res)
{
    if (_length == NULL || _value_res == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_value_res);
    JSON_object* head = json_object_get_object(object_res, REST_NAME_HEAD);

    *_length = 0;
    size_t length = json_object_get_length(head);
    if (length == JSON_DEFAULT_LENGTH) {
        return mma(nak);
    }
    for (size_t ii = 0; ii < length; ++ii) {
        const c8* name = json_object_get_name(head, ii);
        const c8* value = json_object_get_string(head, name);
        *_length += strlen(name) + strlen(REST_DELIMITER_NAME_VALUE) + strlen(value) + strlen(REST_DELIMITER_REST_HEAD);
    }

    *_length += strlen(REST_DELIMITER_REST_HEAD);

    return mma(ack);
}

rcode rest_get_length_res_body(size_t* _length, JSON_value* _value_res, bool _pretty, size_t _indent)
{
    if (_length == NULL || _value_res == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_value_res);
    JSON_object* body = json_object_get_object(object_res, REST_NAME_BODY);

    if (body == NULL || json_object_get_length(body) == 0) {
        *_length = 0;
    }
    else {
        *_length = json_print_get_length_buffer(json_object_get_value_wrapper(body), _pretty, _indent);
    }

    return mma(ack);
}


rcode rest_write_response_line(c8* _line_res, JSON_value* _value_res)
{
    if (_line_res == NULL || _value_res == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_value_res);
    JSON_object* line = json_object_get_object(object_res, REST_NAME_LINE);

    const c8* scheme = json_object_get_string(line, REST_NAME_SCHEME);
    if (scheme == NULL) {
        return mma(nak);
    }
    i32 code;
    if (json_object_has_name(line, REST_NAME_CODE) == false) {
        code = REST_DEFAULT_CODE;
    }
    else {
        code = (i32)json_object_get_number(line, REST_NAME_CODE);
    }
    const c8* message = json_object_get_string(line, REST_NAME_MESSAGE);
    if (message == NULL) {
        return mma(nak);
    }

    sprintf(_line_res, "%s %d %s%s", scheme, code, message, REST_DELIMITER_REST_HEAD);

    return mma(ack);
}

rcode rest_write_response_head(c8* _line_res, JSON_value* _value_res)
{
    if (_line_res == NULL || _value_res == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_value_res);
    JSON_object* head = json_object_get_object(object_res, REST_NAME_HEAD);

    size_t length = json_object_get_length(head);
    if (length == JSON_DEFAULT_LENGTH) {
        return mma(nak);
    }
    for (size_t ii = 0; ii < length; ++ii) {
        const c8* name = json_object_get_name(head, ii);
        const c8* value = json_object_get_string(head, name);
        sprintf(_line_res + strlen(_line_res), "%s%s%s%s", name, REST_DELIMITER_NAME_VALUE, value, REST_DELIMITER_REST_HEAD);
    }

    sprintf(_line_res + strlen(_line_res), "%s", REST_DELIMITER_REST_HEAD);

    return mma(ack);
}

rcode rest_write_response_body(c8* _line_res, JSON_value* _value_res, bool _pretty, size_t _indent)
{
    if (_line_res == NULL || _value_res == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_value_res);
    JSON_object* body = json_object_get_object(object_res, REST_NAME_BODY);

    if (body == NULL) {
    }
    else {
        if (json_object_get_length(body) == 0) {
        }
        else {
            char* buffer = json_print_buffer(json_object_get_value_wrapper(body), _pretty, _indent);
            sprintf(_line_res + strlen(_line_res), "%s", buffer);
            json_free(buffer);
        }
    }

    return mma(ack);
}
