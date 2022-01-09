#include "testIDExt.h"


item_type* search_1234(FSM_event* _evt)
{
    item_type* type = memloc(sizeof(item_type));
    type->id = 18;
    string_memset_print(type->name, sizeof(type->name), "%s", "ExType");
    type->use = 118;

    return type;
}

item_cube* search_5678(FSM_event* _evt)
{
    item_cube* cube = memloc(sizeof(item_cube));
    cube->id = 15;
    string_memset_print(cube->name, sizeof(cube->name), "%s", "ExCube");
    cube->length = 60;
    cube->width = 50;
    cube->height = 40;

    return cube;
}

fsm_rcode get_volume(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    item_type* type = search_1234(_evt);
    item_cube* cube = search_5678(_evt);

    JSON_value* value_res = (JSON_value*)_evt->param0;
    JSON_value* value_req = (JSON_value*)_evt->param1;
    JSON_object* object_req = json_value_get_object(value_req);
    JSON_object* object_req_line = json_object_get_object(object_req, REST_NAME_LINE);
    const c8* scheme = json_object_get_string(object_req_line, REST_NAME_SCHEME);

    rest_set_value_res_line(value_res, REST_RESPONSE_CODE_200, scheme);
    rest_set_value_res_head(value_res, "header", "volume");

    rest_set_value_res_body_number(value_res, "value", cube->length * cube->width * cube->height);

    memfree(type);
    memfree(cube);

    return FSM_RC_ACK;
}

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
