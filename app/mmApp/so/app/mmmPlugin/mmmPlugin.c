#include "mmmPluginExt.h"


// rcode process_message_queue_mmp(c8* _msg, FSM* _fsm)
// {
//     if (_msg == NULL || _fsm == NULL) {
//         return mma(nak);
//     }
//     if (___mmp_context == NULL) {
//         mmp_context_create();
//     }

//     lerror(l_app, "$$$$$ process message mmp");

//     i32 command = *(i32*)_msg;
//     if (command >= MMP_REQ_BASE && command < (MMP_RES_BASE + MM_SIZE_RR_PLUGIN)) {
//         mmp_rr* rr = (mmp_rr*)_msg;
//         ID_event* maker = &rr->common.maker;
//         ID_event* sender = &rr->common.sender;

//         FSM_event* evt = fsm_event_create(FSM_EVENT_DEFAULT, maker, _fsm, NULL, rr, NULL, NULL, NULL);

//         if (command >= MMP_REQ_BASE && command < MMP_RES_BASE) {
//             linfo(l_app, LOGGING_HEAD_MARK " Get NEW Event[MQ] " LOGGING_HEAD_MARK " command(mmp req): [%d], maker: [%lu:%d], sender: [%lu:%d]", command, maker->id, maker->number, sender->id, sender->number);
//             const i8* req_string = json_array_get_string(json_value_get_array(___mmp_context->requests), command - MMP_REQ_BASE);
//             if (req_string == NULL) {
//                 fsm_event_destroy(evt);
//                 lerror(l_app, "Three is not such command(mmp req)");
//                 return mma(nak);
//             }
//             string_memset_print(evt->name, sizeof(evt->name), "%s", req_string);
//         }
//         else if (command >= MMP_RES_BASE && command < MMP_RES_BASE + MM_SIZE_RR_PLUGIN) {
//             linfo(l_app, LOGGING_HEAD_MARK " Get NEW Event[MQ] " LOGGING_HEAD_MARK " command(mmp res): [%d], maker: [%lu:%d], sender: [%lu:%d]", command, maker->id, maker->number, sender->id, sender->number);
//             const i8* res_string = json_array_get_string(json_value_get_array(___mmp_context->responses), command - MMP_RES_BASE);
//             if (res_string == NULL) {
//                 fsm_event_destroy(evt);
//                 lerror(l_app, "Three is not such command(mmp res)");
//                 return mma(nak);
//             }
//             string_memset_print(evt->name, sizeof(evt->name), "%s", res_string);
//         }

//         fsm_fsm_set_item_state_current(evt->fsm, fsm_fsm_get_item_state_entry(evt->fsm));
//         fsm_execute_event_throughout(evt);

//         fsm_event_destroy(evt);

//         ldebug(l_app, "Command maker: [%lu:%d], sender: [%lu:%d], rcode: [%d]", maker->id, maker->number, sender->id, sender->number, ack);
//         return mma(ack);
//     }
//     else {
//         lerror(l_app, "Invalid command: [%d], rcode: [%d]", command, nak);
//         return(nak);
//     }
// }


rcode plugin_set_req(FSM_event* _evt)
{
    if (_evt == NULL || _evt->param1 != NULL || _evt->param2 != NULL) {
        return mma(nak);
    }

    i32 fd_client;
    if ((fd_client = domain_socket_server_accept(theApp.sds.dsock.fd)) < 0) {
        lerror(l_app, "Fail to accept domain socket connection fd[%d]", fd_client);
        return mma(nak);
    }
    ldebug(l_app, "Accepted domain socket connection fd[%d]", fd_client);

    _evt->param1 = memloc(sizeof(mmp_rr));
    i32 fd_received;
    if (domain_socket_receive_msghdr(&fd_received, fd_client, _evt->param1, sizeof(mmp_rr)) < 0) {
        lerror(l_app, "%s", "Fail to receive socket descriptor");
        return mma(nak);
    }
    close(fd_client);

    _evt->param2 = memloc(sizeof(i32));
    *(i32*)_evt->param2 = fd_received;

    return mma(ack);
}

rcode plugin_process(FSM_event* _evt)
{
    if (_evt == NULL || _evt->fsm == NULL || _evt->param1 == NULL || _evt->param0 != NULL) {
        return mma(nak);
    }

    mmp_rr* rr = (mmp_rr*)_evt->param1;
    i32 command = rr->command;
    ID_event maker = rr->common.maker;
    ID_event sender = rr->common.sender;

    const c8* command_string = get_mmp_rr_cmd_strings(command);
    linfo(l_app, LOGGING_HEAD_MARK " Get NEW Event[MQ] " LOGGING_HEAD_MARK " command(mmp): [%d:%s], maker: [%lu:%d], sender: [%lu:%d]", command, command_string, maker.id, maker.number, sender.id, sender.number);
    string_memset_print(_evt->name, sizeof(_evt->name), "%s", command_string);

    fsm_fsm_set_item_state_current(_evt->fsm, fsm_fsm_get_item_state_entry(_evt->fsm));
    fsm_execute_event_throughout(_evt);

    return mma(ack);
}

rcode plugin_send_outcome(FSM_event* _evt)
{
    return rest_send_outcome(_evt, false, 0);
}

rcode plugin_variable_free(FSM_event* _evt)
{
    if (_evt != NULL) {
        if (_evt->param0 != NULL) {
            json_value_free(_evt->param0);
        }
        if (_evt->param1 != NULL) {
            memfree(_evt->param1);
        }
        if (_evt->param2 != NULL) {
            memfree(_evt->param2);
        }
        fsm_event_destroy(_evt);
    }

    return mma(ack);
}
