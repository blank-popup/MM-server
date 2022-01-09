#ifndef ___SO_SUB_THREAD_DEF_H___
#define ___SO_SUB_THREAD_DEF_H___

#include "../../../mmmAppExt.h"
#include "../../lib/mmmServerVariable/mmmServerVariableExt.h"
#include "../../lib/rest/restExt.h"
#include "../mmmPlugin/mmmPluginExt.h"


#ifdef __cplusplus
extern "C" {
#endif

    rcode initialize_common_sub_thread(JSON_array* _array_sub_thread, i32 _type, i32 _ith);
    rcode initialize_server_sub_thread(JSON_array* _array_sub_thread, i32 _type, i32 _ith);
    void* thread_Logger(void* _arg);
    void* thread_Dispatcher(void* _arg);
    void* thread_Server(void* _arg);
    void* thread_Worker(void* _arg);
    rcode main_thread_process();

    rcode process_epoll_message_queue(struct epoll_event* _events, i32 _ii, arg_thread* _arg);
    rcode process_epoll_socket(struct epoll_event* _events, i32 _ii, arg_thread* _arg);
    rcode process_epoll_domain_socket(struct epoll_event* _events, i32 _ii, arg_thread* _arg);

    rcode trace_return(rcode _rc, JSON_object* _logger, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number);
    rcode trace_return_nak(rcode _rc, JSON_object* _logger, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number);
    rcode trace_return_app(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number);
    rcode trace_return_app_nak(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number);
    rcode trace_return_so(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number);
    rcode trace_return_so_nak(rcode _rc, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line_number);

#ifdef __cplusplus
}
#endif

#endif // ___SO_SUB_THREAD_DEF_H___
