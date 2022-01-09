#include "mmmAppExt.h"


void run_daemon(c8* _filepath, c8* _argv[])
{
    execv(_filepath, _argv);
}

int run_process(i32 _argc, c8* _argv[])
{
    // _argc: 4
    // _argv[0]: command
    // _argv[1]: directory root
    // _argv[2]: filepath(config, mm.json)
    // _argv[3]: number
    // _argv[4]: directory python

    if (set_theApp_path(_argc, _argv) < ack) {
        fprintf(stderr, "Fail to set path\n");
        return 1;
    }
    c8 filepath_mm_json[MM_SIZE_PATH] = { 0 };
    string_memset_print(filepath_mm_json, sizeof(filepath_mm_json), "%s/%s", theApp.path.root, theApp.path.config);
    if (set_theApp_config(filepath_mm_json) < ack) {
        fprintf(stderr, "Not found config[%s]\n", filepath_mm_json);
        return 2;
    }

    i32 number = (i32)strtol(_argv[3], NULL, 10);
    if (set_theApp_tag(number) < ack) {
        fprintf(stderr, "Fail to set tag\n");
        return 3;
    }
    size_t length_command = 0;
    for (i32 ii = 0; ii < _argc; ++ii) {
        length_command += strlen(_argv[ii]) + 1;
    }
    for (i32 ii = 1; ii < _argc; ++ii) {
        memset(_argv[ii], 0x00, strlen(_argv[ii]));
    }
    const c8* name_process = get_mmApp_self_name_process();
    string_memset_print(_argv[0], length_command + 3, "%s", name_process);

    if (set_app_action_in_return() < ack) {
        fprintf(stderr, "Fail to set return\n");
        return 3;
    }

    const c8* filepath_log = get_mmApp_self_filepath_log();
    if (set_theApp_logger(theApp.path.root, filepath_log, theApp.self.type, theApp.self.ith) < ack) {
        fprintf(stderr, "Fail to set theApp logger\n");
        return 7;
    }

    linfo(l_app, "%s %s %s", LOGGING_START_MARK, LOGGING_START_MARK, LOGGING_START_MARK);
    linfo(l_app, "Directory root: [%s]", theApp.path.root);
    linfo(l_app, "Filepath json: [%s]", filepath_mm_json);
    linfo(l_app, "Process name: [%s]", name_process);
    linfo(l_app, "Self number: [%d], type: [%d], ith: [%d]", theApp.self.number, theApp.self.type, theApp.self.ith);
    linfo(l_app, "Next number: [%d], type: [%d], ith: [%d]", theApp.next.number, theApp.next.type, theApp.next.ith);
    log_fflush();

    signal(SIGILL, handler_EXODUS);
    signal(SIGABRT, handler_EXODUS);
    signal(SIGBUS, handler_EXODUS);
    signal(SIGFPE, handler_EXODUS);
    signal(SIGSEGV, handler_EXODUS);
    signal(SIGTERM, handler_EXODUS);
    signal(SIGSYS, handler_EXODUS);

    signal(SIGPIPE, handler_SIGPIPE);

    signal(SIGUSR1, handler_SIGUSR1);
    signal(SIGUSR2, handler_SIGUSR2);

    JSON_array* array_sub_thread = get_mmApp_self_sub_thread();
    if (array_sub_thread == NULL || initialize_mmApp(array_sub_thread, theApp.self.type, theApp.self.ith) < ack) {
        lerror(l_app, "Fail to initialize sub_thread app number[%d] type[%s]", theApp.self.number, get_mmApp_self_name());
        return 10;
    }

    action_main_thread main_thread = get_mmApp_self_main_thread();
    if (main_thread == NULL || main_thread() < ack) {
        lerror(l_app, "Fail to execute main thread app number[%d] type[%s]", theApp.self.number, get_mmApp_self_name());
        return 11;
    }
    linfo(l_app, "%s %s initialized %s", LOGGING_START_MARK, name_process, LOGGING_START_MARK);
    log_fflush();

    size_t total_count_sub_thread = get_mmApp_self_total_count_sub_thread();
    f64 period = (f64)get_mmApp_self_timer_period_work() / 1000;
    struct timeval time_entry;
    gettimeofday(&time_entry, NULL);

    while (true) {
        run_periodically();

        struct timeval time_current;
        gettimeofday(&time_current, NULL);
        f64 time_differnece = time_convert_timeval_to_double(time_sub_timeval(time_current, time_entry));
        f64 time_remainder = time_differnece - (u64)(time_differnece / period) * period;
        u64 time_sleep = (u64)((period - time_remainder) * 1000000);
        usleep((useconds_t)time_sleep);
    }

    log_json_finalize();
    free_theApp_config();
    exit(EXIT_SUCCESS);

    return 0;
}

rcode set_theApp_config(const c8* _filepath_mm_json)
{
    theApp.config_root_value = json_value_create_object();
    if (theApp.config_root_value == NULL) {
        fprintf(stderr, "Fail to create config value\n");
        return mma(nak);
    }
    JSON_object* config_root_object = json_value_get_object(theApp.config_root_value);
    JSON_value* common_value = json_parse_file(_filepath_mm_json);
    json_object_set_value(config_root_object, NAME_CONFIG_COMMON, common_value);
    theApp.common_object = json_value_get_object(common_value);

    const c8* process_logger = get_mm_process_logger();
    i32 type_logger = process_name_get_type(process_logger);
    i32 number_logger = process_get_number(type_logger, 0);
    set_mm_number_process_logger(number_logger);

    return mma(ack);
}

rcode free_theApp_config()
{
    json_value_free(theApp.config_root_value);
    return mma(ack);
}

rcode set_theApp_tag(i32 _number)
{
    tag_process self = process_number_get_tag(_number);
    theApp.self.number = self.number;
    theApp.self.type = self.type;
    theApp.self.ith = self.ith;

    i32 number_next = process_number_get_next(self.number);
    tag_process next = process_number_get_tag(number_next);
    theApp.next.number = next.number;
    theApp.next.type = next.type;
    theApp.next.ith = next.ith;

    return mma(ack);
}

rcode set_theApp_path(i32 _argc, c8* _argv[])
{
    string_memset_print(theApp.path.command, sizeof(theApp.path.command), "%s", _argv[0]);
    string_memset_print(theApp.path.root, sizeof(theApp.path.root), "%s", _argv[1]);
    string_memset_print(theApp.path.config, sizeof(theApp.path.config), "%s", _argv[2]);
    string_memset_print(theApp.path.python, sizeof(theApp.path.python), "%s", _argv[4]);

    return mma(ack);
}

rcode set_app_action_in_return()
{
    ___return_app = get_mmApp_self_return_app();
    if (___return_app == NULL) {
        ldebug(l_app, "action app return does not exist");
    }
    ___return_so = get_mmApp_self_return_so();
    if (___return_so == NULL) {
        ldebug(l_app, "action so return does not exist");
    }

    return mma(ack);
}

rcode set_theApp_logger(const c8* _directory_root, const c8* _filepath_log, i32 _type, i32 _ith)
{
    if (_directory_root == NULL || _filepath_log == NULL) {
        fprintf(stderr, "Fail to get directory root\n");
        return mma(nak);
    }

    const c8* type = process_type_get_name(_type);
    JSON_value* value_logger = log_json_intialize(_directory_root, _filepath_log, type, _ith);
    JSON_object* config_root_object = json_value_get_object(theApp.config_root_value);
    json_object_set_value(config_root_object, NAME_CONFIG_LOGGER, value_logger);
    theApp.logger_object = json_object_get_object(config_root_object, NAME_CONFIG_LOGGER);
    if (theApp.logger_object == NULL) {
        fprintf(stderr, "Cannot configure logger[%d][%d]\n", theApp.self.type, theApp.self.ith);
        return mma(nak);
    }

    JSON_object* logger_object_root = json_object_get_object(json_object_get_object(theApp.logger_object, LOG_NAME_LOGGERS), LOG_NAME_ROOT);
    l_app = json_object_get_object(json_object_get_object(logger_object_root, LOG_NAME_LOGGERS), LOGGER_TAG_APP);
    l_so = json_object_get_object(json_object_get_object(logger_object_root, LOG_NAME_LOGGERS), LOGGER_TAG_SO);
    if (l_app == NULL || l_so == NULL) {
        fprintf(stderr, "Fail to set global logger variable\n");
        return mma(nak);
    }

    return mma(ack);
}


rcode initialize_mmApp(JSON_array* _array_sub_thread, i32 _type, i32 _ith)
{
    if (_array_sub_thread == NULL) {
        return mma(nak);
    }
    if ((theApp.sds.ep = epoll_create(SIZE_EPOLL_EVENT)) < 0) {
        return mma(nak);
    }
    linfo(l_app, "Created epoll fd[%d]", theApp.sds.ep);

    if (initialize_app_message_queue_server() < ack) {
        return mma(nak);
    }
    if (initialize_app_message_queue_client() < ack) {
        return mma(nak);
    }
    if (initialize_app_socket_server() < ack) {
        return mma(nak);
    }
    if (initialize_app_dsock_receive_fd_server() < ack) {
        return mma(nak);
    }
    if (initialize_app_dsock_send_fd_client() < ack) {
        return mma(nak);
    }
    if (initialize_app_domain_socket_server() < ack) {
        return mma(nak);
    }
    if (initialize_app_domain_socket_client() < ack) {
        return mma(nak);
    }
    if (initialize_app_named_pipe_server() < ack) {
        return mma(nak);
    }

    action_initialize_sub_thread initialize_sub_thread = get_mmApp_self_initialize_sub_thread();
    if (initialize_sub_thread == NULL || initialize_sub_thread(_array_sub_thread, _type, _ith) < ack) {
        return mma(nak);
    }

    return mma(ack);
}

rcode set_theApp_tag_pid()
{
    theApp.self.pid = getpid();
    linfo(l_app, "Set self pid: [%d]", theApp.self.pid);

    i32 number_next = process_number_get_next(theApp.self.number);
    linked_list_item* item_next = linked_list_get_ith(theApp.cds.mq.list, (size_t)number_next);
    mq_info* info_next = linked_list_item_get_data(item_next);
    mm_rr rrc_req = mm_rr_make(NULL, number_next, true, NULL);
    mma_rr req = mma_rr_make(MMA_REQ_NUMBER_PID, rrc_req);
    mqueue_send(info_next->fd, &req, sizeof(mma_rr), 0);
    ldebug(l_app, "Send message [%s] to number[%d]", get_mma_rr_cmd_strings(MMA_REQ_NUMBER_PID), number_next);

    i32 number_previous = process_number_get_previous(theApp.self.number);
    linked_list_item* item_previous = linked_list_get_ith(theApp.cds.mq.list, (size_t)number_previous);
    mq_info* info_previous = linked_list_item_get_data(item_previous);
    mm_rr rrc_res = mm_rr_make(NULL, number_previous, false, NULL);
    mma_rr res = mma_rr_make(MMA_RES_NUMBER_PID, rrc_res);
    res.what.res_number_pid.pid = theApp.self.pid;
    mqueue_send(info_previous->fd, &res, sizeof(mma_rr), 0);
    ldebug(l_app, "Send message [%s] to number[%d]", get_mma_rr_cmd_strings(MMA_RES_NUMBER_PID), number_previous);

    return mma(ack);
}


rcode set_app_control_teminate(bool _able)
{
    pthread_mutex_lock(&theApp.control.mutex_terminate);
    theApp.control.terminate = _able;
    pthread_mutex_unlock(&theApp.control.mutex_terminate);

    return mma(ack);
}

rcode get_app_control_teminate(bool* _able)
{
    pthread_mutex_lock(&theApp.control.mutex_terminate);
    *_able = theApp.control.terminate;
    pthread_mutex_unlock(&theApp.control.mutex_terminate);

    return mma(ack);
}

rcode generate_app_control_id(size_t* _id)
{
    pthread_mutex_lock(&theApp.control.mutex_id);
    *_id = theApp.control.id++;
    pthread_mutex_unlock(&theApp.control.mutex_id);

    return mma(ack);
}

bool check_have_to_revive_process_next()
{
    const c8* name_partial = process_type_get_name(theApp.next.type);
    i32 status = process_check_pid_status(theApp.next.pid, name_partial);
    if (status == PROCESS_STATUS_UNKNOWN) {
        return false;
    }
    else if (status == PROCESS_STATUS_NOT_EXIST) {
        return true;
    }
    else if (status == PROCESS_STATUS_ACITVE) {
        return false;
    }
    else if (status == PROCESS_STATUS_REVIVING) {
        return false;
    }
    else {
        lerror(l_app, "%s", MM_MESSAGE_UNREACHABLE_CODE);
        return false;
    }
}

void revive_process(i32 _number)
{
    pid_t pid_daemon;
    c8 number[MM_SIZE_NAME] = { 0 };
    string_memset_print(number, sizeof(number), "%d", _number);
    c8* args_daemon[] = { theApp.path.command, theApp.path.root, theApp.path.config, number, theApp.path.python, NULL };
    i32 size_args_daemon = 6;
    c8 filepath[MM_SIZE_PATH] = { 0 };
    string_memset_print(filepath, sizeof(filepath), "%s/%s", theApp.path.root, theApp.path.command);
    process_daemonize(&pid_daemon, run_daemon, filepath, size_args_daemon, args_daemon, NULL, NULL, NULL, NULL);
    linfo(l_app, "Revive daemon number: [%d] pid: [%d]", _number, pid_daemon);
    log_fflush();
}

int check_restart_threads_timeout(threadpool* _pool)
{
    if (_pool == NULL) {
        return 0;
    }

    int restart = 0;
    struct timeval time_current;
    gettimeofday(&time_current, NULL);
    linked_list* threads = tpool_threadpool_get_threads(_pool);
    linked_list_item* item = linked_list_get_head(threads);
    while (item != NULL) {
        linked_list_item* item_next = linked_list_item_get_next(item);
        threadpool_thread* thread = (threadpool_thread*)linked_list_item_get_data(item);
        thread_task* task = tpool_thread_get_task(thread);

        if (tpool_check_thread_timeout(task, time_current) == true) {
            arg_thread* arg = tpool_task_get_arg(task);
            ID_event* maker = arg->evt->maker;
            lerror(l_app, "Thread timeout: maker: [%lu:%d] ", maker->id, maker->number);
            tpool_restart_thread(_pool, task);
            ++restart;
        }
        item = item_next;
    }

    return restart;
}

void run_periodically()
{
    if (check_have_to_revive_process_next() == true) {
        i32 number = process_number_get_next(theApp.self.number);
        revive_process(number);
    }

    check_restart_threads_timeout(theApp.thread_pool);
}

void handler_EXODUS(i32 _sig)
{
    lerror(l_app, "Handle signal[%d][%s]", _sig, strsignal(_sig));

    mma_req_logging req_logging;
    req_logging.event = LOGGING_EVENT_PROCESS_TERMINATED;
    req_logging.level = ll_fatal;
    req_logging.number = theApp.self.number;
    req_logging.pid = theApp.self.pid;
    write_impotant_logging(req_logging);

    signal(_sig, SIG_DFL);
    raise(_sig);
}

void handler_SIGPIPE(i32 _sig)
{
    lerror(l_app, "Handle signal[%d][%s]", _sig, strsignal(_sig));
}

void handler_SIGUSR1(i32 _sig)
{
    lerror(l_app, "Handle signal[%d][%s]", _sig, strsignal(_sig));
}

void handler_SIGUSR2(i32 _sig)
{
    lerror(l_app, "Handle signal[%d][%s]", _sig, strsignal(_sig));
}
