#ifndef ___MMM_APP_JSON_H___
#define ___MMM_APP_JSON_H___

#include "mmmAppType.h"
#include "kit/json/mmJson.h"


#ifdef __cplusplus
extern "C" {
#endif

#define NAME_RETURN_APP "return_app"
#define NAME_RETURN_SO "return_so"

#define NAME_MM_APPS "mmApps"
#define NAME_NAME_PROCESS "name_process"
#define NAME_COUNT_PROCESS "count_process"
#define NAME_INITIALIZE_SUB_THREAD "initialize_sub_thread"
#define NAME_SUB_THREAD "sub_thread"
#define NAME_COUNT_THREAD "count_thread"
#define NAME_FILEPATH_FSM "filepath_fsm"
#define NAME_MAIN_THREAD "main_thread"
// #define NAME_PROCESS_MESSAGE_QUEUE_MMP "process_message_queue_mmp"
#define NAME_TIMER "timer"
#define NAME_PERIOD_WORK "period_work"
#define NAME_COUNT_WAIT_REVIVE "count_wait_revive"
#define NAME_TIMEOUT_THREAD "timeout_thread"
#define NAME_NAME "name"
#define NAME_PATH "path"
#define NAME_MESSAGE_QUEUE "message_queue"
#define NAME_MAXMSG "maxmsg"
#define NAME_MSGSIZE "msgsize"
#define NAME_SHARED_MEMORY "shared_memory"
#define NAME_NAME_SHM "name_shm"
#define NAME_NAME_SEM "name_sem"
#define NAME_SOCKET "socket"
#define NAME_PORT "port"
#define NAME_BACKLOG "backlog"
#define NAME_TYPE_TARGET "type_target"
#define NAME_DSOCK_RECEIVE_FD "dsock_receive_fd"
#define NAME_DOMAIN_SOCKET "domain_socket"
#define NAME_NAMED_PIPE "named_pipe"

#define JSON_MACRO_PREFIX "?"
#define JSON_MACRO_YYYYMMDD "yyyymmdd"
#define JSON_MACRO_HHMMSS "hhmmss"
#define JSON_MACRO_TYPE "type"
#define JSON_MACRO_ITH "##"

    tag_process process_get_tag(i32 _type, i32 _ith);
    tag_process process_number_get_tag(i32 _number);
    i32 process_get_number(i32 _type, i32 _ith);
    i32 process_number_get_next(i32 _number);
    i32 process_number_get_previous(i32 _number);
    i32 process_number_get_type(i32 _number);
    i32 process_number_get_ith(i32 _number);
    i32 process_name_get_type(const c8* _name);
    const c8* process_type_get_name(i32 _type);
    i32 thread_get_number(i32 _type, i32 _ith);
    i32 thread_number_get_type(i32 _number);
    i32 thread_number_get_ith(i32 _number);

    // rcode json_replace_phrase_with_information(c8* _phrase1, size_t _size, const c8* _phrase0, const c8* _type_process, i32 _ith_process);

    const c8* get_mm_directory_root();
    const c8* get_mm_filename_main();
    // const c8* get_mm_filepath_log();
    // action_return get_mm_return_app();
    // action_return get_mm_return_so();
    const c8* get_mm_process_logger();
    void set_mm_number_process_logger(i32 _number);
    i32 get_mm_number_process_logger();

    size_t get_mm_count_processes();
    JSON_object* get_mmApp(i32 _type);
    JSON_object* get_mmApp_self();
    const c8* get_mmApp_name(i32 _type);
    const c8* get_mmApp_self_name();
    const c8* get_mmApp_name_process(i32 _type, i32 _ith);
    const c8* get_mmApp_self_name_process();
    const c8* get_mmApp_filepath_log(i32 _type);
    const c8* get_mmApp_self_filepath_log();
    size_t get_mmApp_count_process(i32 _type);
    size_t get_mmApp_self_count_process();
    action_initialize_sub_thread get_mmApp_initialize_sub_thread(i32 _type);
    action_initialize_sub_thread get_mmApp_self_initialize_sub_thread();
    JSON_array* get_mmApp_sub_thread(i32 _type);
    JSON_array* get_mmApp_self_sub_thread();
    size_t get_mmApp_total_count_sub_thread(i32 _type);
    size_t get_mmApp_self_total_count_sub_thread();

    JSON_object* get_mmApp_self_logger();

    action_main_thread get_mmApp_main_thread(i32 _type);
    action_main_thread get_mmApp_self_main_thread();
    action_return get_mmApp_return_app(i32 _type);
    action_return get_mmApp_self_return_app();
    action_return get_mmApp_return_so(i32 _type);
    action_return get_mmApp_self_return_so();

    time_t get_mmApp_timer_period_work(i32 _type);
    time_t get_mmApp_self_timer_period_work();
    i32 get_mmApp_timer_count_wait_revive(i32 _type);
    i32 get_mmApp_self_timer_count_wait_revive();
    time_t get_mmApp_timer_timeout_thread(i32 _type);
    time_t get_mmApp_self_timer_timeout_thread();
    const c8* get_mmApp_message_queue_name(i32 _type, i32 _ith);
    const c8* get_mmApp_self_message_queue_name();
    __syscall_slong_t get_mmApp_message_queue_maxmsg(i32 _type);
    __syscall_slong_t get_mmApp_self_message_queue_maxmsg();
    __syscall_slong_t get_mmApp_message_queue_msgsize(i32 _type);
    __syscall_slong_t get_mmApp_self_message_queue_msgsize();
    const c8* get_mmApp_shared_memory_name_shm(i32 _type, i32 _ith);
    const c8* get_mmApp_self_shared_memory_name_shm();
    const c8* get_mmApp_shared_memory_name_sem(i32 _type, i32 _ith);
    const c8* get_mmApp_self_shared_memory_name_sem();
    u16 get_mmApp_socket_port(i32 _type, i32 _ith);
    u16 get_mmApp_self_socket_port();
    i32 get_mmApp_socket_backlog(i32 _type);
    i32 get_mmApp_self_socket_backlog();
    const c8* get_mmApp_socket_type_target(i32 _type, i32 _ith);
    const c8* get_mmApp_self_socket_name_target();
    const c8* get_mmApp_dsock_receive_fd_path(i32 _type, i32 _ith);
    const c8* get_mmApp_self_dsock_receive_fd_path();
    i32 get_mmApp_dsock_receive_fd_backlog(i32 _type);
    i32 get_mmApp_self_dsock_receive_fd_backlog();
    const c8* get_mmApp_domain_socket_path(i32 _type, i32 _ith);
    const c8* get_mmApp_self_domain_socket_path();
    i32 get_mmApp_domain_socket_backlog(i32 _type);
    i32 get_mmApp_self_domain_socket_backlog();
    const c8* get_mmApp_named_pipe_path(i32 _type, i32 _ith);
    const c8* get_mmApp_self_named_pipe_path();

#ifdef __cplusplus
    }
#endif

#endif // ___MMM_APP_JSON_H___
