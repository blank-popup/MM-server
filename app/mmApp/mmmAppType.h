#ifndef ___MMM_APP_TYPE_H___
#define ___MMM_APP_TYPE_H___

#include "base/mmBaseType.h"
#include "base/mmEtc.h"
#include "base/mmMemory.h"
#include "base/mmString.h"
#include "base/mmTime.h"
#include "base/mmFile.h"
#include "base/mmIPC.h"
#include "base/mmSocket.h"
#include "base/mmMultiplex.h"
#include "kit/process/mmProcess.h"
#include "kit/URL/mmURL.h"
#include "kit/log/mmLog.h"
#include "kit/json/mmJson.h"
#include "kit/FSM/mmFSM.h"
#include "kit/thread/mmThread.h"
#include "kit/structure/mmLinkedList.h"
#include "kit/structure/mmCircularLinkedList.h"


#ifdef __cplusplus
extern "C" {
#endif

#define mma(rc) areturn(rc, ___return_app, "APP", __FILE__, __func__, __LINE__)
#define mms(rc) areturn(rc, ___return_so, "SO", __FILE__, __func__, __LINE__)

#define SIZE_EPOLL_EVENT (1024)

    typedef struct _ID_event {
        i32 number;
        size_t id;
    } ID_event;

    typedef struct _ID_pt {
        i32 type;
        i32 ith;
    } ID_pt;

    typedef struct _path_mm {
        c8 command[MM_SIZE_NAME];
        c8 root[MM_SIZE_PATH];
        c8 config[MM_SIZE_PATH];
        c8 python[MM_SIZE_PATH];
    } path_mm;

    typedef struct _tag_process {
        i32 number;
        i32 type;
        i32 ith;
        pid_t pid;
    } tag_process;

#define MAX_COUNT_PROCESS (32)
#define MAX_COUNT_THREAD (32)
    typedef struct _thread_info {
        ID_pt ID;
        i32 number;
        bool running;
        struct timeval time_start;
        struct timeval time_finish;
    } thread_info;

    typedef struct _arg_thread {
        ID_pt ID;
        FSM* fsm;
        FSM_event* evt;
    } arg_thread;

    typedef struct _mq_info {
        i32 number;
        mqd_t fd;
        __syscall_slong_t maxmsg;
        __syscall_slong_t msgsize;
    } mq_info;

    typedef struct _sock_info {
        i32 number;
        i32 fd;
    } sock_info;

    typedef struct _dsock_server_info {
        i32 number;
        i32 fd;
    } dsock_server_info;

    typedef struct _dsock_client_info {
        i32 number;
        c8 filepath_server[MM_SIZE_PATH];
    } dsock_client_info;

    typedef struct _np_info {
        i32 number;
        i32 fd;
    } np_info;

    typedef struct _server_desctiptor {
        i32 ep;
        mq_info mq;
        i32 shm;
        sem_t* sem;
        sock_info sock;
        dsock_server_info recvfd;
        dsock_server_info dsock;
        np_info np;
    } server_desctiptor;

    typedef struct _message_queue_client {
        linked_list* list;
        size_t size;
    } message_queue_client;

    typedef struct _sendfd_client {
        linked_list* list;
        size_t size;
        linked_list_item* item_latest;
    } sendfd_client;

    typedef struct _dsock_client {
        linked_list* list;
        size_t size;
        linked_list_item* item_latest;
    } dsock_client;

    typedef struct _client_desctiptor {
        message_queue_client mq;
        sendfd_client sendfd;
        dsock_client dsock;
    } client_desctiptor;

    typedef struct _controller {
        bool terminate;
        pthread_mutex_t mutex_terminate;
        size_t id;
        pthread_mutex_t mutex_id;
    } controller;

#define NAME_CONFIG_COMMON "COMMON"
#define NAME_CONFIG_LOGGER "LOGGER"

    typedef struct _config_application {
        path_mm path;
        JSON_value* config_root_value;
        JSON_object* common_object;
        JSON_object* logger_object;
        tag_process self;
        tag_process next;
        threadpool* thread_pool;
        linked_list* values_fsm;
        linked_list* tasks;
        server_desctiptor sds;
        client_desctiptor cds;
        controller control;
    } config_application;

    typedef rcode(*action_initialize_sub_thread)(JSON_array*, i32, i32);
    typedef void*(*action_sub_thread)(void*);
    typedef rcode(*action_main_thread)(void);

#ifdef __cplusplus
    }
#endif

#endif // ___MMM_APP_TYPE_H___
