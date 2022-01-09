#include "restExt.h"


rcode rest_set_value_req(FSM_event* _evt)
{
    if (_evt == NULL || _evt->param1 != NULL || _evt->param2 == NULL) {
        return mma(nak);
    }

    c8* sock_req = NULL;
    size_t size = SOCKET_SIZE_RECV_BUFFER;
    if (rest_blocking_recv_all(&sock_req, &size, *(i32*)_evt->param2) < ack) {
        return mma(nak);
    }

    lerror(l_app, "$$$$$ [%s]", sock_req);

    _evt->param1 = json_value_create_object();
    JSON_object* object_req = json_value_get_object(_evt->param1);
    JSON_value* value_line = NULL;
    if (rest_set_value_req_line(&value_line, sock_req) < ack) {
        return mma(nak);
    }
    JSON_value* value_head = NULL;
    if (rest_set_value_req_head(&value_head, sock_req) < ack) {
        return mma(nak);
    }
    JSON_value* value_body = NULL;
    if (rest_set_value_req_body(&value_body, sock_req) < ack) {
        return mma(nak);
    }

    json_object_set_value(object_req, REST_NAME_LINE, value_line);
    json_object_set_value(object_req, REST_NAME_HEAD, value_head);
    json_object_set_value(object_req, REST_NAME_BODY, value_body);

    memfree(sock_req);

    c8* line_req = json_print_buffer(_evt->param1, true, 4);
    ldebug(l_app, "%s", line_req);
    json_free(line_req);

    return mma(ack);
}

static const c8* get_event_from_path(FSM_event* _evt, JSON_object* _object_req_line)
{
    if (_object_req_line == NULL) {
        return NULL;
    }

    const c8* method = json_object_get_string(_object_req_line, REST_NAME_METHOD);
    JSON_array* path = json_object_get_array(_object_req_line, REST_NAME_PATH);

    size_t length = json_array_get_length(path);
    JSON_object* rest = json_value_get_object(mm_rest_get());
    if (length == 1 && strcmp("", json_array_get_string(path, 0)) == 0) {
    }
    else {
        for (size_t ii = 0; ii < length; ++ii) {
            const c8* path_ith = json_array_get_string(path, ii);
            if (path_ith == NULL) {
                return NULL;
            }
            JSON_object* rest_next = json_object_get_object(rest, path_ith);
            if (rest_next == NULL) {
                if (json_object_has_name(rest, PLUGIN_NAME_ACTION_ID) == true) {
                    if (_evt->param3 == NULL) {
                        _evt->param3 = json_value_create_array();
                    }
                    JSON_value* value_searching = _evt->param3;
                    JSON_array* array_searching = json_value_get_array(value_searching);
                    json_array_append_string(array_searching, path_ith);
                }
                else {
                    return NULL;
                }
                rest = json_object_get_object(rest, PLUGIN_NAME_ACTION_ID);
            }
            else {
                rest = rest_next;
            }
        }
    }
    c8* name_method = memloc((strlen(REST_NAME_HEAD_METHOD) + strlen(method) + 1) * sizeof(char));
    sprintf(name_method, REST_NAME_HEAD_METHOD "%s", method);
    c8* evt = (c8*)json_object_get_string(rest, name_method);
    memfree(name_method);

    return evt;
}

rcode rest_process(FSM_event* _evt)
{
    if (_evt == NULL || _evt->fsm == NULL || _evt->param1 == NULL || _evt->param0 != NULL) {
        return mma(nak);
    }

    _evt->param0 = json_value_create_object();

    if (rest_req_line_is_valid(_evt->param1) == false) {
        rest_set_value_res_line(_evt->param0, REST_RESPONSE_CODE_404, REST_DEFAULT_SCHEME);
        return mma(ack);
    }
    else {
        JSON_object* object_req = json_value_get_object(_evt->param1);
        JSON_object* object_req_line = json_object_get_object(object_req, REST_NAME_LINE);

        const c8* req_string = get_event_from_path(_evt, object_req_line);
        if (req_string == NULL) {
            const c8* scheme = json_object_get_string(object_req_line, REST_NAME_SCHEME);
            rest_set_value_res_line(_evt->param0, REST_RESPONSE_CODE_404, scheme);
            return mma(ack);
        }
        string_memset_print(_evt->name, sizeof(_evt->name), "%s", req_string);

        fsm_fsm_set_item_state_current(_evt->fsm, fsm_fsm_get_item_state_entry(_evt->fsm));
        fsm_rcode rc = fsm_execute_event_throughout(_evt);

        if (rc != NULL && strcmp(rc, PLUGIN_RC_ACK_PASS_REQUEST) == 0) {
            return mma(ack_pass_request);
        }
        else if (rc != NULL && strcmp(rc, FSM_RC_ACK) == 0) {
            return mma(ack);
        }
        else {
            const c8* method = json_object_get_string(object_req_line, REST_NAME_METHOD);
            JSON_array* array_path = json_object_get_array(object_req_line, REST_NAME_PATH);
            c8* path = rest_path_convert_array_to_string(array_path);
            lerror(l_app, "Cannot execute REST api: [%s] %s", method, path);
            memfree(path);

            const c8* scheme = json_object_get_string(object_req_line, REST_NAME_SCHEME);
            rest_set_value_res_line(_evt->param0, REST_RESPONSE_CODE_404, scheme);
            return mma(ack);
        }
    }
}

rcode rest_send_outcome(FSM_event* _evt, bool _pretty, size_t _indent)
{
    if (_evt == NULL || _evt->param0 == NULL) {
        return mma(nak);
    }

    JSON_object* object_res = json_value_get_object(_evt->param0);
    JSON_object* line = json_object_get_object(object_res, REST_NAME_LINE);
    i32 code = (i32)json_object_get_number(line, REST_NAME_CODE);
    i32 code_100 = code / 100;

    size_t length_res_line = 0;
    size_t length_res_head = 0;
    size_t length_res_body = 0;

    if (code_100 == 1) {

    }
    else if (code_100 == 2) {
        rest_get_length_res_body(&length_res_body, _evt->param0, _pretty, _indent);
        rest_get_length_res_line(&length_res_line, _evt->param0);
        c8 content_length[REST_SIZE_CONTENT_LENGTH] = { 0 };
        sprintf(content_length, "%lu", length_res_body);
        rest_set_value_res_head(_evt->param0, REST_NAME_CONTENT_LENGTH, content_length);
        rest_get_length_res_head(&length_res_head, _evt->param0);
        c8* line_res = memloc((length_res_line + length_res_head + length_res_body + 1) * sizeof(char));
        rest_write_response_line(line_res, _evt->param0);
        rest_write_response_head(line_res, _evt->param0);
        rest_write_response_body(line_res, _evt->param0, _pretty, _indent);

        rest_blocking_send_all(line_res, strlen(line_res), *(i32*)_evt->param2);
        memfree(line_res);
        return mma(ack);
    }
    else if (code_100 == 3) {

    }
    else if (code_100 == 4) {
        rest_get_length_res_line(&length_res_line, _evt->param0);
        c8* line_res = memloc((length_res_line + strlen(REST_DELIMITER_REST_HEAD) + 1) * sizeof(char));
        rest_write_response_line(line_res, _evt->param0);
        sprintf(line_res + strlen(line_res), "%s", REST_DELIMITER_REST_HEAD);

        rest_blocking_send_all(line_res, strlen(line_res), *(i32*)_evt->param2);
        memfree(line_res);
        return mma(ack);
    }
    else if (code_100 == 5) {

    }

    return mma(nak);
}

rcode rest_variable_free(FSM_event* _evt)
{
    if (_evt != NULL) {
        if (_evt->maker != NULL) {
            memfree(_evt->maker);
        }
        if (_evt->param0 != NULL) {
            json_value_free(_evt->param0);
        }
        if (_evt->param1 != NULL) {
            json_value_free(_evt->param1);
        }
        if (_evt->param2 != NULL) {
            close(*(i32*)_evt->param2);
            memfree(_evt->param2);
        }
        if (_evt->param3 != NULL) {
            json_value_free(_evt->param3);
        }
        fsm_event_destroy(_evt);
    }

    return mma(ack);
}
