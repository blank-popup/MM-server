#include "subThreadExt.h"


rcode process_epoll_message_queue(struct epoll_event* _events, i32 _ii, arg_thread* _arg)
{
    c8 msg[theApp.sds.mq.msgsize];
    memset(msg, 0x00, theApp.sds.mq.msgsize);
    if (mqueue_receive(theApp.sds.mq.fd, msg, theApp.sds.mq.msgsize, NULL) < 0) {
        lerror(l_app, "%s", "Fail to receive message");
        return mma(nak);
    }

    process_message_queue(msg);

    return mma(ack);
}

rcode process_epoll_socket(struct epoll_event* _events, i32 _ii, arg_thread* _arg)
{
    epoll_ptr_info* epoll_ptr = (epoll_ptr_info*)_events[_ii].data.ptr;
    ID_event* maker = memloc(sizeof(ID_event));
    memcpy(maker, &epoll_ptr->maker, sizeof(ID_event));
    _arg->evt = fsm_event_create(FSM_EVENT_DEFAULT, maker, _arg->fsm, NULL, NULL, NULL, NULL, NULL);
    _arg->evt->param2 = memloc(sizeof(i32));
    *(i32*)_arg->evt->param2 = epoll_ptr->fd;
    memfree(epoll_ptr);
    rest_set_value_req(_arg->evt);
    rcode rc = rest_process(_arg->evt);
    if (rc == ack) {
        rest_send_outcome(_arg->evt, false, 0);
        rest_variable_free(_arg->evt);
    }
    else if (rc == ack_pass_request) {
        rest_variable_free(_arg->evt);
    }
    else {
        rest_variable_free(_arg->evt);
    }

    return mma(ack);
}

rcode process_epoll_domain_socket(struct epoll_event* _events, i32 _ii, arg_thread* _arg)
{
    _arg->evt = fsm_event_create(FSM_EVENT_DEFAULT, NULL, _arg->fsm, NULL, NULL, NULL, NULL, NULL);
    plugin_set_req(_arg->evt);
    plugin_process(_arg->evt);
    plugin_send_outcome(_arg->evt);
    plugin_variable_free(_arg->evt);

    return mma(ack);
}


rcode trace_return(rcode _rc, JSON_object* _logger, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number)
{
    if (_logger != NULL) {
        if (_rc >= ack) {
            ltrace(_logger, "[%s] [ACK] %s - (%s:%d)", _tag, _filepath, _function, _line_number);
        }
        else {
            ltrace(_logger, "[%s] [NAK] %s - (%s:%d)", _tag, _filepath, _function, _line_number);
        }
    }

    return _rc;
}

rcode trace_return_nak(rcode _rc, JSON_object* _logger, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number)
{
    if (_logger != NULL) {
        if (_rc < ack) {
            ltrace(_logger, "[%s] [NAK] %s - (%s:%d)", _tag, _filepath, _function, _line_number);
        }
    }

    return _rc;
}

rcode trace_return_app(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number)
{
    return trace_return(_rc, l_app, _tag, _filepath, _function, _line_number);
}

rcode trace_return_app_nak(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number)
{
    return trace_return_nak(_rc, l_app, _tag, _filepath, _function, _line_number);
}

rcode trace_return_so(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number)
{
    return trace_return(_rc, l_so, _tag, _filepath, _function, _line_number);
}

rcode trace_return_so_nak(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number)
{
    return trace_return_nak(_rc, l_so, _tag, _filepath, _function, _line_number);
}
