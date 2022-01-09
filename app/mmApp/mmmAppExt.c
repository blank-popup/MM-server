#include "mmmAppDef.h"


config_application theApp = {
    { 0, },                 // path_mm path
    NULL,                   // JSON_value* config_root_value
    NULL,                   // JSON_object* common_object
    NULL,                   // JSON_object* logger_object
    // tag_process tag
    {
        -1,                 // i32 number
        -1,                 // i32 type
        -1,                 // i32 ith_process
        0                   // pid_t pid
    },
    // tag_process next
    {
        -1,                 // i32 number
        -1,                 // i32 type
        -1,                 // i32 ith_process
        0                   // pid_t pid
    },
    NULL,                   // threadpool* thread_pool
    NULL,                   // linked_list* values_fsm
    NULL,                   // linked_list* tasks
    // server_desctiptor sds
    {
        -1,                 // i32 ep
        { 0 },              // mq_info mq
        -1,                 // i32 shm
        NULL,               // sem_t* sem
        { 0 },              // sock_info sock
        { 0 },              // dsock_server_info recvfd
        { 0 },              // dsock_server_info dsock
        { 0 }               // np_info np
    },
    // client_desctiptor cds
    {
        { 0 },              // message_queue_client mq
        { 0 },              // sendfd_client sendfd
        { 0 }               // dsock_client dsock
    },
    // controller control
    {
        false,                                  // bool terminate
        PTHREAD_MUTEX_INITIALIZER,              // pthread_mutex_t mutex_terminate
        0,                                      // size_t id
        PTHREAD_MUTEX_INITIALIZER               // pthread_mutex_t mutex_id
    }
};

action_return ___return_app = NULL;
action_return ___return_so = NULL;

JSON_object* l_app = NULL;
JSON_object* l_so = NULL;
