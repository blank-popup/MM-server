#include "mmmAppExt.h"


tag_process process_get_tag(i32 _type, i32 _ith)
{
    tag_process tag = { -1, -1, -1, 0 };

    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    i32 number = 0;
    size_t length_mmApps = json_object_get_length(mmApps);
    if (_type >= (i32)length_mmApps) {
        return tag;
    }
    for (size_t ii = 0; ii < length_mmApps; ++ii) {
        JSON_object* mmApp = json_object_get_ith_object(mmApps, ii);
        size_t count_process = json_object_get_real_number(mmApp, NAME_COUNT_PROCESS);
        if (_type == (i32)ii && _ith >= (i32)count_process) {
            return tag;
        }
        for (size_t jj = 0; jj < count_process; ++jj) {
            if (_type == (i32)ii && _ith == (i32)jj) {
                return (tag_process){number, _type, _ith, 0};
            }
            else {
                ++number;
            }
        }
    }

    return tag;
}

tag_process process_number_get_tag(i32 _number)
{
    tag_process tag = { -1, -1, -1, 0 };
    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    i32 number = 0;
    size_t length_mmApps = json_object_get_length(mmApps);
    for (size_t ii = 0; ii < length_mmApps; ++ii) {
        JSON_object* mmApp = json_object_get_ith_object(mmApps, ii);
        size_t count_process = json_object_get_real_number(mmApp, NAME_COUNT_PROCESS);
        for (size_t jj = 0; jj < count_process; ++jj) {
            if (_number == number) {
                return (tag_process){ _number, (i32)ii, (i32)jj, 0};
            }
            else {
                ++number;
            }
        }
    }

    return tag;
}

i32 process_get_number(i32 _type, i32 _ith)
{
    tag_process tag = process_get_tag(_type, _ith);
    return tag.number;
}

i32 process_number_get_next(i32 _number)
{
    i32 number = _number + 1;
    i32 count_processes = (i32)get_mm_count_processes();
    if (number >= count_processes) {
        number = 0;
    }

    return number;
}

i32 process_number_get_previous(i32 _number)
{
    i32 number = _number - 1;
    if (number < 0) {
        i32 count_processes = (i32)get_mm_count_processes();
        number = count_processes - 1;
    }

    return number;
}

i32 process_number_get_type(i32 _number)
{
    tag_process tag = process_number_get_tag(_number);
    return tag.type;
}

i32 process_number_get_ith(i32 _number)
{
    tag_process tag = process_number_get_tag(_number);
    return tag.ith;
}

i32 process_name_get_type(const c8* _name)
{
    if (_name == NULL) {
        return -1;
    }

    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    i32 type = 0;
    JSON_object_item* item = json_object_get_item_head(mmApps);
    while (item != NULL) {
        const c8* name = json_object_item_get_name(item);
        if (name != NULL && strcmp(_name, name) == 0) {
            return type;
        }
        ++type;
        item = json_object_item_get_next(item);
    }

    return -1;
}

const c8* process_type_get_name(i32 _type)
{
    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    return json_object_get_name(mmApps, _type);
}

i32 thread_get_number(i32 _type, i32 _ith)
{
    if (_type < 0 || _ith < 0) {
        return -1;
    }

    JSON_array* array_sub_thread = get_mmApp_self_sub_thread();
    size_t count_type = json_array_get_length(array_sub_thread);
    if ((size_t)_type >= count_type) {
        return -1;
    }

    i32 number = 0;
    for (i32 ii = 0; ii < _type; ++ii) {
        JSON_object* type_thread = json_array_get_object(array_sub_thread, (size_t)ii);
        number += (i32)json_object_get_real_number(type_thread, NAME_COUNT_THREAD);
    }
    JSON_object* self_type_thread = json_array_get_object(array_sub_thread, (size_t)_type);
    size_t count_thread = json_object_get_real_number(self_type_thread, NAME_COUNT_THREAD);
    if ((size_t)_ith >= count_thread) {
        return -1;
    }

    number += _ith;

    return number;
}

i32 thread_number_get_type(i32 _number)
{
    if (_number < 0) {
        return -1;
    }

    JSON_array* array_sub_thread = get_mmApp_self_sub_thread();
    size_t count_type = json_array_get_length(array_sub_thread);
    size_t total_count_thread = 0;
    for (size_t ii = 0; ii < count_type; ++ii) {
        JSON_object* type_thread = json_array_get_object(array_sub_thread, ii);
        size_t count_thread = json_object_get_real_number(type_thread, NAME_COUNT_THREAD);
        total_count_thread += count_thread;
        if (_number < total_count_thread) {
            return ii;
        }
    }

    return -1;
}

i32 thread_number_get_ith(i32 _number)
{
    if (_number < 0) {
        return -1;
    }

    JSON_array* array_sub_thread = get_mmApp_self_sub_thread();
    size_t count_type = json_array_get_length(array_sub_thread);
    i32 total_count_thread = 0;
    for (i32 ii = 0; ii < count_type; ++ii) {
        JSON_object* type_thread = json_array_get_object(array_sub_thread, (size_t)ii);
        i32 count_thread = (i32)json_object_get_real_number(type_thread, NAME_COUNT_THREAD);
        if (_number < total_count_thread + count_thread) {
            i32 ith = _number - total_count_thread;
            return ith;
        }
        total_count_thread += count_thread;
    }

    return -1;
}


static JSON_value* get_value_mm_object(const c8* _name)
{
    JSON_object* mm = json_object_get_object(theApp.common_object, NAME_MM);
    JSON_object_item* item = json_object_get_object_item(mm, _name);
    return json_object_item_get_value(item);
}

static JSON_value* get_value_mm_object_object(const c8* _name0, const c8* _name1)
{
    JSON_object* mm = json_object_get_object(theApp.common_object, NAME_MM);
    JSON_object* object = json_object_get_object(mm, _name0);
    JSON_object_item* item = json_object_get_object_item(object, _name1);
    return json_object_item_get_value(item);
}

static JSON_value* get_value_mm_object_array(const c8* _name, size_t _ith)
{
    JSON_object* mm = json_object_get_object(theApp.common_object, NAME_MM);
    JSON_array* array = json_object_get_array(mm, _name);
    JSON_array_item* item = json_array_get_array_item(array, _ith);
    return json_array_item_get_value(item);
}

static JSON_value* get_value_mmApp_object(i32 _type, const c8* _name)
{
    JSON_object* mmApp = get_mmApp(_type);
    JSON_object_item* item = json_object_get_object_item(mmApp, _name);
    return json_object_item_get_value(item);
}

static JSON_value* get_value_mmApp_object_object(i32 _type, const c8* _name0, const c8* _name1)
{
    JSON_object* mmApp = get_mmApp(_type);
    JSON_object* object = json_object_get_object(mmApp, _name0);
    JSON_object_item* item = json_object_get_object_item(object, _name1);
    return json_object_item_get_value(item);
}

static JSON_value* get_value_mmApp_object_array(i32 _type, const c8* _name, size_t _ith)
{
    JSON_object* mmApp = get_mmApp(_type);
    JSON_array* array = json_object_get_array(mmApp, _name);
    JSON_array_item* item = json_array_get_array_item(array, _ith);
    return json_array_item_get_value(item);
}

static JSON_value* get_value_mmApp_object_object_array(i32 _type, const c8* _name0, const c8* _name1, size_t _ith)
{
    JSON_object* mmApp = get_mmApp(_type);
    JSON_object* object = json_object_get_object(mmApp, _name0);
    JSON_array* array = json_object_get_array(object, _name1);
    JSON_array_item* item = json_array_get_array_item(array, _ith);
    return json_array_item_get_value(item);
}


// rcode json_replace_phrase_with_information(c8* _phrase1, size_t _size, const c8* _phrase0, const c8* _type_process, i32 _ith_process)
// {
//     struct timeval val;
//     gettimeofday(&val, NULL);
//     struct tm* t = localtime(&val.tv_sec);
//     char yyyymmdd[LOG_SIZE_YYYYMMDD] = { 0, };
//     char hhmmss[LOG_SIZE_HHMMSS] = { 0, };

//     string_memset_print(yyyymmdd, sizeof(yyyymmdd), "%04d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
//     string_memset_print(hhmmss, sizeof(hhmmss), "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);

//     c8* phrase1 = string_replace_all(_phrase0, JSON_MACRO_PREFIX JSON_MACRO_TYPE, _type_process);
//     c8 ith[MM_SIZE_NAME] = { 0 };
//     string_memset_print(ith, sizeof(ith), "%02d", _ith_process);
//     c8* phrase2 = string_replace_all(phrase1, JSON_MACRO_PREFIX JSON_MACRO_ITH, ith);
//     memfree(phrase1);
//     c8* phrase3 = string_replace_all(phrase2, JSON_MACRO_PREFIX JSON_MACRO_YYYYMMDD, yyyymmdd);
//     memfree(phrase2);
//     c8* phrase4 = string_replace_all(phrase3, JSON_MACRO_PREFIX JSON_MACRO_HHMMSS, hhmmss);
//     memfree(phrase3);
//     string_memset_print(_phrase1, _size, "%s", phrase4);
//     memfree(phrase4);

//     return ack;
// }


const c8* get_mm_directory_root()
{
    JSON_value* value = get_value_mm_object(NAME_DIRECTORY_ROOT);
    return json_value_get_string(value);
}

const c8* get_mm_filename_main()
{
    JSON_value* value = get_value_mm_object(NAME_FILENAME_MAIN);
    return json_value_get_string(value);
}

const c8* get_mm_directory_python()
{
    JSON_value* value = get_value_mm_object(NAME_DIRECTORY_PYTHON);
    return json_value_get_string(value);
}

// const c8* get_mm_filepath_log()
// {
//     JSON_value* value = get_value_mm_object(NAME_FILEPATH_LOG);
//     return json_value_get_string(value);
// }

// action_return get_mm_return_app()
// {
//     JSON_value* value = get_value_mm_object(NAME_RETURN_APP);
//     JSON_array* array = json_value_get_array(value);
//     action_return action = (action_return)fsm_get_action(array, get_mm_directory_root());
//     return action;
// }

// action_return get_mm_return_so()
// {
//     JSON_value* value = get_value_mm_object(NAME_RETURN_SO);
//     JSON_array* array = json_value_get_array(value);
//     action_return action = (action_return)fsm_get_action(array, get_mm_directory_root());
//     return action;
// }

const c8* get_mm_process_logger()
{
    JSON_value* value = get_value_mm_object(NAME_PROCESS_LOGGER);
    return json_value_get_string(value);
}

void set_mm_number_process_logger(i32 _number)
{
    JSON_object* mm = json_object_get_object(theApp.common_object, NAME_MM);
    json_object_set_number(mm, NAME_NUMBER_PROCESS_LOGGER, (double)_number);
}

i32 get_mm_number_process_logger()
{
    JSON_value* value = get_value_mm_object(NAME_NUMBER_PROCESS_LOGGER);
    return (i32)json_value_get_real_number(value);
}


size_t get_mm_count_processes()
{
    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    size_t count = 0;
    for (size_t ii = 0; ii < json_object_get_length(mmApps); ++ii) {
        const c8* name = json_object_get_name(mmApps, ii);
        JSON_object* mmApp = json_object_get_object(mmApps, name);
        size_t count_mmApp = (size_t)json_object_get_real_number(mmApp, NAME_COUNT_PROCESS);
        count += count_mmApp;
    }

    return count;
}

JSON_object* get_mmApp(i32 _type)
{
    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    return json_object_get_ith_object(mmApps, _type);
}

JSON_object* get_mmApp_self()
{
    return get_mmApp(theApp.self.type);
}

const c8* get_mmApp_name(i32 _type)
{
    JSON_object* mmApps = json_object_get_object(theApp.common_object, NAME_MM_APPS);
    return json_object_get_name(mmApps, _type);
}

const c8* get_mmApp_self_name()
{
    return get_mmApp_name(theApp.self.type);
}

const c8* get_mmApp_name_process(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_array(_type, NAME_NAME_PROCESS, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_name_process()
{
    return get_mmApp_name_process(theApp.self.type, theApp.self.ith);
}

const c8* get_mmApp_filepath_log(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_FILEPATH_LOG);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_filepath_log()
{
    return get_mmApp_filepath_log(theApp.self.type);
}

size_t get_mmApp_count_process(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_COUNT_PROCESS);
    return (size_t)json_value_get_real_number(value);
}

size_t get_mmApp_self_count_process()
{
    return get_mmApp_count_process(theApp.self.type);
}

action_initialize_sub_thread get_mmApp_initialize_sub_thread(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_INITIALIZE_SUB_THREAD);
    JSON_array* array = json_value_get_array(value);
    action_initialize_sub_thread action = (action_initialize_sub_thread)fsm_get_action(array, get_mm_directory_root());
    return action;
}

action_initialize_sub_thread get_mmApp_self_initialize_sub_thread()
{
    return get_mmApp_initialize_sub_thread(theApp.self.type);
}

JSON_array* get_mmApp_sub_thread(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_SUB_THREAD);
    return json_value_get_array(value);
}

JSON_array* get_mmApp_self_sub_thread()
{
    return get_mmApp_sub_thread(theApp.self.type);
}

size_t get_mmApp_total_count_sub_thread(i32 _type)
{
    JSON_array* array_sub_thread = get_mmApp_sub_thread(_type);
    size_t count_type = json_array_get_length(array_sub_thread);
    size_t total_count = 0;
    for (size_t ii = 0; ii < count_type; ++ii) {
        JSON_object* type_thread = json_array_get_object(array_sub_thread, ii);
        total_count += (size_t)json_object_get_real_number(type_thread, NAME_COUNT_THREAD);
    }

    return total_count;
}

size_t get_mmApp_self_total_count_sub_thread()
{
    return get_mmApp_total_count_sub_thread(theApp.self.type);
}


JSON_object* get_mmApp_self_logger()
{
    return theApp.logger_object;
}

action_main_thread get_mmApp_main_thread(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_MAIN_THREAD);
    JSON_array* array = json_value_get_array(value);
    action_main_thread action = (action_main_thread)fsm_get_action(array, get_mm_directory_root());
    return action;
}

action_main_thread get_mmApp_self_main_thread()
{
    return get_mmApp_main_thread(theApp.self.type);
}

action_return get_mmApp_return_app(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_RETURN_APP);
    JSON_array* array = json_value_get_array(value);
    action_return action = (action_return)fsm_get_action(array, get_mm_directory_root());
    return action;
}

action_return get_mmApp_self_return_app()
{
    return get_mmApp_return_app(theApp.self.type);
}

action_return get_mmApp_return_so(i32 _type)
{
    JSON_value* value = get_value_mmApp_object(_type, NAME_RETURN_SO);
    JSON_array* array = json_value_get_array(value);
    action_return action = (action_return)fsm_get_action(array, get_mm_directory_root());
    return action;
}

action_return get_mmApp_self_return_so()
{
    return get_mmApp_return_so(theApp.self.type);
}

time_t get_mmApp_timer_period_work(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_TIMER, NAME_PERIOD_WORK);
    return (time_t)json_value_get_real_number(value);
}

time_t get_mmApp_self_timer_period_work()
{
    return get_mmApp_timer_period_work(theApp.self.type);
}

i32 get_mmApp_timer_count_wait_revive(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_TIMER, NAME_COUNT_WAIT_REVIVE);
    return (time_t)json_value_get_real_number(value);
}

i32 get_mmApp_self_timer_count_wait_revive()
{
    return get_mmApp_timer_period_work(theApp.self.type);
}

time_t get_mmApp_timer_timeout_thread(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_TIMER, NAME_TIMEOUT_THREAD);
    return (time_t)json_value_get_real_number(value);
}

time_t get_mmApp_self_timer_timeout_thread()
{
    return get_mmApp_timer_timeout_thread(theApp.self.type);
}

const c8* get_mmApp_message_queue_name(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_MESSAGE_QUEUE, NAME_NAME, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_message_queue_name()
{
    return get_mmApp_message_queue_name(theApp.self.type, theApp.self.ith);
}

__syscall_slong_t get_mmApp_message_queue_maxmsg(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_MESSAGE_QUEUE, NAME_MAXMSG);
    return (__syscall_slong_t)json_value_get_real_number(value);
}

__syscall_slong_t get_mmApp_self_message_queue_maxmsg()
{
    return get_mmApp_message_queue_maxmsg(theApp.self.type);
}

__syscall_slong_t get_mmApp_message_queue_msgsize(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_MESSAGE_QUEUE, NAME_MSGSIZE);
    return (__syscall_slong_t)json_value_get_real_number(value);
}

__syscall_slong_t get_mmApp_self_message_queue_msgsize()
{
    return get_mmApp_message_queue_msgsize(theApp.self.type);
}

const c8* get_mmApp_shared_memory_name_shm(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_SHARED_MEMORY, NAME_NAME_SHM, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_shared_memory_name_shm()
{
    return get_mmApp_shared_memory_name_shm(theApp.self.type, theApp.self.ith);
}

const c8* get_mmApp_shared_memory_name_sem(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_SHARED_MEMORY, NAME_NAME_SEM, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_shared_memory_name_sem()
{
    return get_mmApp_shared_memory_name_sem(theApp.self.type, theApp.self.ith);
}

u16 get_mmApp_socket_port(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_SOCKET, NAME_PORT, _ith);
    return (u16)json_value_get_real_number(value);
}

u16 get_mmApp_self_socket_port()
{
    return get_mmApp_socket_port(theApp.self.type, theApp.self.ith);
}

i32 get_mmApp_socket_backlog(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_SOCKET, NAME_BACKLOG);
    return (i32)json_value_get_real_number(value);
}

i32 get_mmApp_self_socket_backlog()
{
    return get_mmApp_socket_backlog(theApp.self.type);
}

const c8* get_mmApp_socket_type_target(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_SOCKET, NAME_TYPE_TARGET, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_socket_name_target()
{
    return get_mmApp_socket_type_target(theApp.self.type, theApp.self.ith);
}

const c8* get_mmApp_dsock_receive_fd_path(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_DSOCK_RECEIVE_FD, NAME_PATH, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_dsock_receive_fd_path()
{
    return get_mmApp_dsock_receive_fd_path(theApp.self.type, theApp.self.ith);
}

i32 get_mmApp_dsock_receive_fd_backlog(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_DSOCK_RECEIVE_FD, NAME_BACKLOG);
    return (i32)json_value_get_real_number(value);
}

i32 get_mmApp_self_dsock_receive_fd_backlog()
{
    return get_mmApp_dsock_receive_fd_backlog(theApp.self.type);
}

const c8* get_mmApp_domain_socket_path(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_DOMAIN_SOCKET, NAME_PATH, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_domain_socket_path()
{
    return get_mmApp_domain_socket_path(theApp.self.type, theApp.self.ith);
}

i32 get_mmApp_domain_socket_backlog(i32 _type)
{
    JSON_value* value = get_value_mmApp_object_object(_type, NAME_DOMAIN_SOCKET, NAME_BACKLOG);
    return (i32)json_value_get_real_number(value);
}

i32 get_mmApp_self_domain_socket_backlog()
{
    return get_mmApp_domain_socket_backlog(theApp.self.type);
}

const c8* get_mmApp_named_pipe_path(i32 _type, i32 _ith)
{
    JSON_value* value = get_value_mmApp_object_object_array(_type, NAME_NAMED_PIPE, NAME_PATH, _ith);
    return json_value_get_string(value);
}

const c8* get_mmApp_self_named_pipe_path()
{
    return get_mmApp_named_pipe_path(theApp.self.type, theApp.self.ith);
}
