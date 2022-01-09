#include "testPythonExt.h"


fsm_rcode server_execute_python(FSM_event* _evt)
{

    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    JSON_value* value_res = (JSON_value*)_evt->param0;
    JSON_value* value_req = (JSON_value*)_evt->param1;
    JSON_object* object_req = json_value_get_object(value_req);
    JSON_object* object_req_line = json_object_get_object(object_req, REST_NAME_LINE);
    const c8* scheme = json_object_get_string(object_req_line, REST_NAME_SCHEME);

    FILE* file = process_popen_python("/home/nova/WorkSpace/MM-REST-server/run/plugin/py/testPython.py", "aaa bbb ccc", true);
    if (file == NULL) {
        rest_set_value_res_line(value_res, REST_RESPONSE_CODE_404, scheme);
        rest_set_value_res_head(value_res, "run", "python");
        return FSM_RC_ACK;
    }

    c8* readings = process_popen_fgets(file, 10);
    pclose(file);
    lerror(l_app, "$$$$ [[[%s]]]", readings);
    memfree(readings);

    rest_set_value_res_line(value_res, REST_RESPONSE_CODE_200, scheme);
    rest_set_value_res_head(value_res, "run", "python");

    return FSM_RC_ACK;
}
