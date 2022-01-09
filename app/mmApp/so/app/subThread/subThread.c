#include "subThreadExt.h"


static void common_arg_thread_free(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    if (arg->evt != NULL) {
    }
}

static void server_arg_thread_free(void* _task)
{
    lerror(l_app, "$$$$$ $$$$$$$$$$$$$$$$$ free");
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    lerror(l_app, "$$$$$ $$$$$$$$$$$$$$$$$ arg [%d][%d]", arg->ID.type, arg->ID.ith);
    if (arg->evt != NULL) {
        rest_variable_free(arg->evt);
    }
}

static void worker_arg_thread_free(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    if (arg->evt != NULL) {
        plugin_variable_free(arg->evt);
    }
}

static void(*get_arg_thread_free(i32 _type))(void*)
{
    const c8* type = process_type_get_name(_type);
    if (type == NULL) {
        return NULL;
    }
    if (strcmp(type, PROCESS_TYPE_LOGGER) == 0) {
        return common_arg_thread_free;
    }
    else if (strcmp(type, PROCESS_TYPE_DISPATCHER) == 0) {
        return common_arg_thread_free;
    }
    else if (strcmp(type, PROCESS_TYPE_SERVER) == 0) {
        return server_arg_thread_free;
    }
    else if (strcmp(type, PROCESS_TYPE_WORKER) == 0) {
        return worker_arg_thread_free;
    }

    return NULL;
}

rcode initialize_common_sub_thread(JSON_array* _array_sub_thread, i32 _type, i32 _ith)
{
    if (_array_sub_thread == NULL) {
        return mma(nak);
    }

    void(*arg_thread_free)(void*) = get_arg_thread_free(_type);
    if (arg_thread_free == NULL) {
        return mma(nak);
    }

    const c8* directory_root = get_mm_directory_root();
    size_t count_type = json_array_get_length(_array_sub_thread);
    size_t total_thread = get_mmApp_self_total_count_sub_thread();

    i32 status_min_thread = STATUS_MIN_THREAD;
    i32 status_max_thread = STATUS_MAX_THREAD;
    theApp.thread_pool = tpool_threadpool_create(status_min_thread, status_max_thread, NULL, NULL, NULL, NULL);
    linfo(l_app, "Created threadpool range thread status[%d - %d]", status_min_thread, status_max_thread);
    theApp.tasks = tpool_tasks_create(total_thread);
    // theApp.targ = memloc(sizeof(arg_thread) * total_thread);

    for (size_t ii = 0; ii < total_thread; ++ii) {
        tpool_threadpool_add_thread(theApp.thread_pool, NULL, NULL);
    }

    size_t kk = 0;
    theApp.values_fsm = linked_list_create();
    for (size_t ii = 0; ii < count_type; ++ii) {
        JSON_object* type_thread = json_array_get_object(_array_sub_thread, ii);
        JSON_array* array_sub_thread = json_object_get_array(type_thread, NAME_SUB_THREAD);
        action_sub_thread sub_thread = fsm_get_action(array_sub_thread, directory_root);

        const c8* filepath_fsm = json_object_get_string(type_thread, NAME_FILEPATH_FSM);
        JSON_value* value_fsm = fsm_parse_fsm_root_value(filepath_fsm, directory_root);
        linked_list_item* item = linked_list_item_create(&value_fsm, sizeof(value_fsm));
        linked_list_append_item(theApp.values_fsm, item);
        // JSON_value* vf = *(JSON_value**)linked_list_item_get_data(item);
        // lerror(l_app, "$$$$$ ======== [%p] [%p]", (void*)value_fsm, (void*)vf);

        size_t count_thread = (size_t)json_object_get_real_number(type_thread, NAME_COUNT_THREAD);
        for (size_t jj = 0; jj < count_thread; ++jj) {
            thread_task* task = tpool_tasks_get_task(theApp.tasks, kk++);
            arg_thread* arg = memloc(sizeof(arg_thread));
            // arg_thread* arg = &theApp.targ[kk++];
            FSM* fsm = fsm_create(value_fsm, NULL);
            FSM_event* evt = NULL;
            arg->ID.type = (i32)ii;
            arg->ID.ith = (i32)jj;
            arg->fsm = fsm;
            arg->evt = evt;
            // arg->expire.tv_sec = 0;
            // arg->expire.tv_usec = 0;
            tpool_task_set_arg(task, arg, arg_thread_free);
            tpool_task_set_action(task, sub_thread, NULL, NULL);
            if (jj == 0) {
                c8* line_fsm = json_print_buffer(value_fsm, true, 4);
                linfo(l_app, LOGGING_HEAD_MARK " FSM " LOGGING_HEAD_MARK "\n%s", line_fsm);
                json_free(line_fsm);
            }
            // tpool_threadpool_add_thread_task(theApp.thread_pool, sub_thread, (void*)arg, arg_thread_free, NULL, NULL);

            tpool_threadpool_add_thread_task(theApp.thread_pool, task);
        }
    }

    return mma(ack);
}

// rcode arg_thread_set(arg_thread* _arg, i32 _id_type, i32 _id_ith, JSON_value* _value_fsm, FSM_event* _evt)
// {
//     if (_arg == NULL || _evt == NULL) {
//         return mma(nak);
//     }

//     _arg->ID.type = _id_type;
//     _arg->ID.ith = _id_ith;
//     _arg->fsm = fsm_create(_value_fsm, NULL);;
//     _arg->evt = _evt;
//     // _arg->expire.tv_sec = _sec;
//     // _arg->expire.tv_usec = _usec;

//     return mma(ack);
// }

rcode initialize_server_sub_thread(JSON_array* _array_sub_thread, i32 _type, i32 _ith)
{
    if (_array_sub_thread == NULL) {
        return mma(nak);
    }

    c8 filepath_immanence[MM_SIZE_PATH] = { 0, };
    const c8* directory_root = get_mm_directory_root();
    const c8* type = process_type_get_name(_type);
    snprintf(filepath_immanence, sizeof(filepath_immanence), "%s/%s/%s/%s", directory_root, DIRNAME_DEFAULT_CONFIG, type, FilENAME_DEFAULT_IMMANENCE);
    size_t count_actions = 0;
    if (file_check_exist(filepath_immanence) == true) {
        if (mm_rest_create(filepath_immanence) < ack) {
            return mma(nak);
        }
    }
    else {
        return mma(nak);
    }

    if (initialize_common_sub_thread(_array_sub_thread, _type, _ith) < ack) {
        return mma(nak);
    }

    return mma(ack);
}

void* thread_Logger(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    ldebug(l_app, "Created thread task[%p] pthread[%u] ID(type:ith)[%d:%d] FSM[%p]", _task, pthread_self(), arg->ID.type, arg->ID.ith, (void*)arg->fsm);
    log_fflush();

    while (true) {
        struct epoll_event events[SIZE_EPOLL_EVENT];
        i32 count_event = epoll_wait(theApp.sds.ep, events, SIZE_EPOLL_EVENT, -1);
        if (count_event < 0) {
            lerror(l_app, "%s", "fail to epoll_wait");
        }

        for (i32 ii = 0; ii < count_event; ++ii) {
            if (events[ii].data.fd == theApp.sds.mq.fd) {
                process_epoll_message_queue(events, ii, arg);
            }

            log_fflush();
        }
    }

    return NULL;
}

void* thread_Dispatcher(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    ldebug(l_app, "Created thread task[%p] pthread[%u] ID(type:ith)[%d:%d] FSM[%p]", _task, pthread_self(), arg->ID.type, arg->ID.ith, (void*)arg->fsm);
    log_fflush();

    while (true) {
        struct epoll_event events[SIZE_EPOLL_EVENT];
        i32 count_event = epoll_wait(theApp.sds.ep, events, SIZE_EPOLL_EVENT, -1);
        if (count_event < 0) {
            lerror(l_app, "%s", "fail to epoll_wait");
        }

        for (i32 ii = 0; ii < count_event; ++ii) {
            if (events[ii].data.fd == theApp.sds.mq.fd) {
                process_epoll_message_queue(events, ii, arg);
            }
            else if (events[ii].data.fd == theApp.sds.sock.fd) {
                accept_send_fd();
            }

            log_fflush();
        }
    }

    return NULL;
}


static void cleanup_thread_server(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    lerror(l_app, "$$$$$ pthread cancel !!!!! task[%p] arg[%p]", (void*)task, (void*)arg);
    server_arg_thread_free(task);

}

void* thread_Server(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    ldebug(l_app, "Created thread task[%p] pthread[%lu] ID(type:ith)[%d:%d] FSM[%p]", _task, pthread_self(), arg->ID.type, arg->ID.ith, (void*)arg->fsm);
    log_fflush();
    time_t timeout_msec = (time_t)get_mmApp_self_timer_timeout_thread();

    pthread_cleanup_push(cleanup_thread_server, (void*)_task);
    while (true) {
        struct epoll_event events[SIZE_EPOLL_EVENT];
        i32 count_event = epoll_wait(theApp.sds.ep, events, SIZE_EPOLL_EVENT, -1);

        if (count_event < 0) {
            lerror(l_app, "%s", "fail to epoll_wait");
        }

        for (i32 ii = 0; ii < count_event; ++ii) {
            struct timeval time_current;
            gettimeofday(&time_current, NULL);
            struct timeval expire = time_add_timeval(time_current, (struct timeval){timeout_msec / 1000, timeout_msec % 1000 * 1000});
            tpool_task_set_expire(task, expire);
            i32 status = tpool_threadpool_get_self_thread_status(theApp.thread_pool);
            tpool_threadpool_set_self_thread_status(theApp.thread_pool, status + 1);

            if (events[ii].data.fd == theApp.sds.mq.fd) {
                // memory_tracer_print("AFTER epoll message queue", get_mmApp_self_name_process());
                process_epoll_message_queue(events, ii, arg);
                // memory_tracer_print("AFTER epoll message queue", get_mmApp_self_name_process());
            }
            else if (events[ii].data.fd == theApp.sds.recvfd.fd) {
                receive_add_epoll_fd();
            }
            else {
                // memory_tracer_print("BEFORE epoll socket", get_mmApp_self_name_process());
                process_epoll_socket(events, ii, arg);
                // memory_tracer_print("AFTER epoll socket", get_mmApp_self_name_process());
            }

            tpool_threadpool_set_self_thread_status(theApp.thread_pool, status);
            tpool_task_set_expire(task, (struct timeval){0, 0});
            log_fflush();
        }
    }
    pthread_cleanup_pop(0);

    return NULL;
}

void* thread_Worker(void* _task)
{
    thread_task* task = (thread_task*)_task;
    arg_thread* arg = tpool_task_get_arg(task);
    ldebug(l_app, "Created thread task[%p] pthread[%u] ID(type:ith)[%d:%d] FSM[%p]", _task, pthread_self(), arg->ID.type, arg->ID.ith, (void*)arg->fsm);
    log_fflush();

    while (true) {
        struct epoll_event events[SIZE_EPOLL_EVENT];
        i32 count_event = epoll_wait(theApp.sds.ep, events, SIZE_EPOLL_EVENT, -1);
        if (count_event < 0) {
            lerror(l_app, "%s", "fail to epoll_wait");
        }

        for (i32 ii = 0; ii < count_event; ++ii) {
            if (events[ii].data.fd == theApp.sds.mq.fd) {
                process_epoll_message_queue(events, ii, arg);
            }
            else if (events[ii].data.fd == theApp.sds.dsock.fd) {
                process_epoll_domain_socket(events, ii, arg);
            }

            log_fflush();
        }
    }

    return NULL;
}


rcode main_thread_process()
{
    if (set_theApp_tag_pid() < ack) {
        return mma(nak);
    }

    return mma(ack);
}
