#ifndef ___SO_REST_DEF_H___
#define ___SO_REST_DEF_H___

#include "../../../mmmAppExt.h"
#include "../mmmServerVariable/mmmServerVariableExt.h"


#ifdef __cplusplus
extern "C" {
#endif

#define REST_DELIMITER_REST_HEAD "\r\n"
#define REST_DELIMITER_REST_HEAD_AND_BODY "\r\n\r\n"
#define REST_DELIMITER_SPACE " "
#define REST_DELIMITER_NAME_VALUE ": "

#define REST_METHOD_OPTIONS "OPTIONS"
#define REST_METHOD_GET "GET"
#define REST_METHOD_HEAD "HEAD"
#define REST_METHOD_POST "POST"
#define REST_METHOD_PUT "PUT"
#define REST_METHOD_DELETE "DELETE"
#define REST_METHOD_TRACE "TRACE"
#define REST_METHOD_CONNECT "CONNECT"

#define REST_SCHEME_HTTP_1_1 "HTTP/1.1"
#define REST_SCHEME_HTTP_2 "HTTP/2"
#define REST_NAME_USER_AGENT "user-agent"
#define REST_NAME_CONTENT_TYPE "content-type"
#define REST_NAME_ACCEPT_ENCODING "accept-encoding"
#define REST_NAME_CONTENT_LENGTH "content-length"
#define REST_NAME_HOST "host"
#define REST_NAME_CONNECTION "connection"

#define REST_NAME_LINE "line"
#define REST_NAME_HEAD "head"
#define REST_NAME_BODY "body"

#define REST_NAME_METHOD "method"
#define REST_NAME_PATH "path"
#define REST_NAME_QUERY "query"
#define REST_NAME_SCHEME "scheme"

#define REST_NAME_CODE "code"
#define REST_NAME_MESSAGE "message"

#define REST_DEFAULT_SCHEME "HTTP/1.1"
#define REST_DEFAULT_CODE (404)

#define REST_CHARACTER_SPACE ' '
#define REST_CHARACTER_SLASH '/'
#define REST_CHARACTER_QUESTION '?'
#define REST_CHARACTER_EQUAL '='
#define REST_CHARACTER_AMPERSAND '&'
#define REST_CHARACTER_COLON ':'
#define REST_CHARACTER_RETURN '\r'
#define REST_CHARACTER_NULL '\0'

#define REST_NAME_HEAD_METHOD "\\/"

typedef enum _code_response {
    REST_RESPONSE_CODE_200 = 200,
    REST_RESPONSE_CODE_201 = 201,
    REST_RESPONSE_CODE_204 = 204,
    REST_RESPONSE_CODE_404 = 404,
    REST_RESPONSE_CODE_500 = 500
} code_response;

#define REST_SIZE_CODE_RESPONES (4)
#define REST_SIZE_CONTENT_LENGTH (64)

#define REST_RESPONSE_LINE_MESSAGE_200 "OK"
#define REST_RESPONSE_LINE_MESSAGE_201 "Created"
#define REST_RESPONSE_LINE_MESSAGE_204 "No Content"
#define REST_RESPONSE_LINE_MESSAGE_404 "Not Found"
#define REST_RESPONSE_LINE_MESSAGE_500 "Internal Server Error"


#define PLUGIN_RC_ACK_PASS_REQUEST "ACK_PASS_REQUEST"

typedef enum _rcode_value_plugin {
    ack_pass_request = 1
} rcode_value_plugin;

// #define PLUGIN_NAME_ACTION_SEARCH "\\\\search"
#define PLUGIN_NAME_ACTION_ID "\\\\"
// #define PLUGIN_NAME_ACTIONS "actions"
// #define PLUGIN_NAME_RESULTS "results"

// FSM_event parameter
//                                      Server                  Worker
// param0: response                     JSON_value*             JSON_value*
// param1: request                      JSON_value*             mmp_rr*
// param2: fd                                                   i32*
// param3: searching id                 JSON_value*             JSON_value*


    // rest.c
    rcode rest_set_value_req(FSM_event* _evt);
    rcode rest_process(FSM_event* _evt);
    rcode rest_send_outcome(FSM_event* _evt, bool _pretty, size_t _indent);
    rcode rest_variable_free(FSM_event* _evt);

    // restTask.c
    rcode rest_blocking_recv_all(c8** _data, size_t* _size, i32 _fd);
    rcode rest_blocking_send_all(c8* _data, size_t _length, i32 _fd);

    const c8* convert_version_to_scheme(i32 _version);
    i32 convert_scheme_to_version(const c8* _scheme);
    c8* rest_path_convert_array_to_string(JSON_array* _path);
    JSON_value* rest_path_convert_string_to_array(c8* _path, size_t _length);

    bool rest_method_is_valid(mstring _method);
    bool rest_req_line_is_valid(JSON_value* _value_req);
    bool rest_res_line_is_valid(JSON_value* _value_res);

    rcode rest_set_value_req_line(JSON_value** _value_line, c8* _sock_req);
    rcode rest_set_value_req_head(JSON_value** _value_head, c8* _sock_req);
    rcode rest_set_value_req_body(JSON_value** _value_body, c8* _sock_req);

    rcode rest_set_value_res_line(JSON_value* _value_res, code_response _code, const c8* _scheme);
    rcode rest_set_value_res_head(JSON_value* _value_res, const c8* _name, const c8* _value);
    rcode rest_set_value_res_body_string(JSON_value* _value_res, const c8* _name, const c8* _value);
    rcode rest_set_value_res_body_number(JSON_value* _value_res, const c8* _name, double _value);

    rcode rest_get_length_res_line(size_t* _length, JSON_value* _value_res);
    rcode rest_get_length_res_head(size_t* _length, JSON_value* _value_res);
    rcode rest_get_length_res_body(size_t* _length, JSON_value* _value_res, bool _pretty, size_t _indent);

    rcode rest_write_response_line(c8* line_res, JSON_value* _value_res);
    rcode rest_write_response_head(c8* line_res, JSON_value* _value_res);
    rcode rest_write_response_body(c8* line_res, JSON_value* _value_res, bool _pretty, size_t _indent);

#ifdef __cplusplus
}
#endif

#endif // ___SO_REST_DEF_H___
