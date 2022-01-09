#include "testServerWorkerExt.h"


fsm_rcode server_send_req_plus_one(FSM_event* _evt)
{

    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    i32 number = 5;

    mm_rr rrc = mm_rr_make((ID_event*)_evt->maker, number, true, NULL);
    JSON_value* value_req = _evt->param1;
    JSON_object* object_req = json_value_get_object(value_req);
    JSON_object* object_line = json_object_get_object(object_req, REST_NAME_LINE);
    const c8* scheme = json_object_get_string(object_line, REST_NAME_SCHEME);
    i32 version = convert_scheme_to_version(scheme);
    if (version == -1) {
        return FSM_RC_NAK;
    }
    mmp_rr req = mmp_rr_make(MMP_REQ_PLUS_ONE, rrc, version);
    JSON_object* object_body = json_object_get_object(object_req, REST_NAME_BODY);
    if (json_object_get_type(object_body, "value") == json_type_number) {
        return FSM_RC_NAK;
    }
    req.what.req_plus_one.value = (i32)json_object_get_real_number(object_body, "value");

    i32 fd_dsock;
    if ((fd_dsock = domain_socket_client_initialize(false)) < 0) {
        lerror(l_app, "%s", "Fail to initialize domain socket client");
        return FSM_RC_NAK;
    }

    linked_list_item* item = linked_list_get_tail(theApp.cds.dsock.list);
    dsock_client_info* info = linked_list_item_get_data(item);
    domain_socket_client_connect(fd_dsock, info->filepath_server);
    lerror(l_app, "Connected to domain socket fd[%d], filepath server[%s]", fd_dsock, info->filepath_server);

    if (domain_socket_send_msghdr(*(i32*)_evt->param2, fd_dsock, &req, sizeof(mmp_rr)) < 0) {
        lerror(l_app, "%s", "Fail to send socket descriptor");
        return FSM_RC_NAK;
    }
    lerror(l_app, "Send msghdr to domain socket fd[%d], filepath server[%s]", fd_dsock, info->filepath_server);
    sleep(20);
    lerror(l_app, "$$$$$ awaken 20");

    return PLUGIN_RC_ACK_PASS_REQUEST;
}

fsm_rcode worker_receive_req_plus_one(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    mmp_rr* req = (mmp_rr*)_evt->param1;
    ID_event* maker = &req->common.maker;
    _evt->maker = maker;

    mm_rr rrc = mm_rr_make(maker, req->common.sender.number, false, NULL);
    lerror(l_app, "$$$$$ ========= [%d %lu]", maker->number, maker->id);

    _evt->param0 = json_value_create_object();
    const c8* scheme = convert_version_to_scheme(req->version);
    if (scheme == NULL) {
        return FSM_RC_NAK;
    }
    rest_set_value_res_line(_evt->param0, REST_RESPONSE_CODE_200, scheme);

    return FSM_RC_ACK;
}

fsm_rcode worker_receive_plus_one_second(FSM_event* _evt)
{
    lerror(l_app, "$$$$$ %s %s %d", __FILE__, __func__, __LINE__);

    mmp_rr* req = (mmp_rr*)_evt->param1;
    JSON_value* res = (JSON_value*)_evt->param0;

    rest_set_value_res_body_number(res,"value", req->what.req_plus_one.value + 1);
    // JSON_object* object_res = json_value_get_object(res);
    // JSON_object* object_body = json_object_get_object(object_res, REST_NAME_BODY);
    // json_object_set_number(object_body, "value", req->what.req_plus_one.value + 1);

    return FSM_RC_ACK;
}
