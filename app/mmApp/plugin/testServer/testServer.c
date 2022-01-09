#include "testServerExt.h"


fsm_rcode help_start(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    JSON_value* value_res = (JSON_value*)_evt->param0;
    JSON_value* value_req = (JSON_value*)_evt->param1;
    JSON_object* object_req = json_value_get_object(value_req);
    JSON_object* object_req_line = json_object_get_object(object_req, REST_NAME_LINE);
    const c8* scheme = json_object_get_string(object_req_line, REST_NAME_SCHEME);

    rest_set_value_res_line(value_res, REST_RESPONSE_CODE_200, scheme);
    rest_set_value_res_head(value_res, "header", "help");

    return FSM_RC_ACK;
}

fsm_rcode basic_start(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    JSON_value* value_res = (JSON_value*)_evt->param0;
    JSON_value* value_req = (JSON_value*)_evt->param1;
    JSON_object* object_req = json_value_get_object(value_req);
    JSON_object* object_req_line = json_object_get_object(object_req, REST_NAME_LINE);
    const c8* scheme = json_object_get_string(object_req_line, REST_NAME_SCHEME);

    rest_set_value_res_line(value_res, REST_RESPONSE_CODE_200, scheme);
    rest_set_value_res_head(value_res, "header", "model");

    return FSM_RC_ACK;
}

fsm_rcode state_0_default(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    JSON_value* value_res = (JSON_value*)_evt->param0;
    JSON_value* value_req = (JSON_value*)_evt->param1;
    rest_set_value_res_body_string(value_res, "subject", "help");

    return FSM_RC_ACK;
}

fsm_rcode state_1_default(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    JSON_value* value_res = (JSON_value*)_evt->param0;
    JSON_value* value_req = (JSON_value*)_evt->param1;
    rest_set_value_res_body_string(value_res, "content", "This is server");

    return FSM_RC_ACK;
}

fsm_rcode state_final_default(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    // JSON_value* value_res = (JSON_value*)_evt->param0;
    // JSON_value* value_req = (JSON_value*)_evt->param1;

    return FSM_RC_ACK;
}
