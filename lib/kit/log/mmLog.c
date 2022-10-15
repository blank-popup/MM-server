#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include "mmLog.h"
#include "kit/structure/mmLinkedList.h"
#include "kit/email/email.h"
#include "../../mmLogSimple.h"


char* log_memset_print(char* _buffer, size_t _size, const char* _message, ...)
{
    if (_buffer == NULL || _message == NULL) {
        return NULL;
    }

    memset(_buffer, 0x00, _size);
    va_list argp;
    va_start(argp, _message);
    vsnprintf(_buffer, _size, _message, argp);
    va_end(argp);

    return _buffer;
}

static double log_convert_pointer_to_double(void* _pointer)
{
    unsigned long address = (unsigned long)_pointer;

    return (double)address;
}

static void* log_convert_double_to_pointer(double _double)
{
    unsigned long address = (unsigned long)_double;

    return (void*)address;
}

typedef struct _logger_handler_consol {
    FILE* stream;
} logger_handler_consol;

typedef struct _logger_handler_file {
    char path0[LOG_SIZE_PATH];
    char directory_root[LOG_SIZE_PATH];
    char name[LOG_SIZE_NAME_PROCESS];
    int ith;
    char path[LOG_SIZE_PATH];
    FILE* stream;
    logger_level critical;
    JSON_array_item* items_time_rotation;
    long capacity_rotation;
    time_t time_rotation_next;
    long capacity_current;
} logger_handler_file;

typedef struct _logger_handler_email {
    char** receivers;
    size_t length;
} logger_handler_email;

typedef union _logger_handler_individual {
    logger_handler_consol consol;
    logger_handler_file file;
    logger_handler_email email;
} logger_handler_individual;

typedef struct _logger_handler {
    char name[LOG_SIZE_NAME_HANDLER];
    logger_level level;
    bool active;
    logger_handler_type type;
    logger_handler_individual u;
} logger_handler;

static JSON_value* ___mm_value_logger = NULL;
static linked_list* ___mm_list_handler = NULL;
static pthread_mutex_t ___mm_mutex_logger = PTHREAD_MUTEX_INITIALIZER;
static sem_t* ___mm_sem_logger = NULL;
static char ___mm_name_sem[LOG_SIZE_NAME_SEM] = { 0 };
static char ___mm_label_logger[LOG_SIZE_LABEL] = { 0 };
static int(*___mm_lock_logger)(void) = NULL;
static int(*___mm_unlock_logger)(void) = NULL;


static void logger_write_time(char* _logtime, struct timeval _time_value)
{
    struct tm* t = localtime(&_time_value.tv_sec);
    sprintf(_logtime, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, _time_value.tv_usec);
}

static void logger_write_level(char* _loglevel, int _level)
{
    if (_level == ll_trace) {
        sprintf(_loglevel, "T");
    } else if (_level == ll_debug) {
        sprintf(_loglevel, "D");
    } else if (_level == ll_info) {
        sprintf(_loglevel, "I");
    } else if (_level == ll_warn) {
        sprintf(_loglevel, "W");
    } else if (_level == ll_error) {
        sprintf(_loglevel, "E");
    } else if (_level == ll_fatal) {
        sprintf(_loglevel, "F");
    } else {
        sprintf(_loglevel, "Lv%d", _level);
    }
}


static int logger_lock_mutex()
{
    return pthread_mutex_lock(&___mm_mutex_logger);
}

static int logger_unlock_mutex()
{
    return pthread_mutex_unlock(&___mm_mutex_logger);
}

static int logger_lock_semaphore()
{
    return sem_wait(___mm_sem_logger);
}

static int logger_unlock_semaphore()
{
    return sem_post(___mm_sem_logger);
}


static logger_level log_convert_name_to_logger_level(const char* _name)
{
    if (_name == NULL) {
        return ll_trace;
    }

    if (strcmp(_name, LOG_VALUE_LEVEL_FATAL) == 0) {
        return ll_fatal;
    }
    else if (strcmp(_name, LOG_VALUE_LEVEL_ERROR) == 0) {
        return ll_error;
    }
    else if (strcmp(_name, LOG_VALUE_LEVEL_WARN) == 0) {
        return ll_warn;
    }
    else if (strcmp(_name, LOG_VALUE_LEVEL_INFO) == 0) {
        return ll_info;
    }
    else if (strcmp(_name, LOG_VALUE_LEVEL_DEBUG) == 0) {
        return ll_debug;
    }
    else if (strcmp(_name, LOG_VALUE_LEVEL_TRACE) == 0) {
        return ll_trace;
    }

    return ll_trace;
}

static FILE* log_convert_name_to_stream(const char* _name)
{
    if (_name == NULL) {
        return stderr;
    }

    if (strcmp(_name, LOG_VALUE_STREAM_STDOUT) == 0) {
        return stdout;
    }

    return stderr;
}


static char* modify_phrase_with_information(char* _result, const char* _source, const char* _name, int _ith)
{
    char yyyymmdd[LOG_SIZE_YYYYMMDD] = { 0, };
    char hhmmss[LOG_SIZE_HHMMSS] = { 0, };
    char uuuuuu[LOG_SIZE_UUUUUU] = { 0, };

    struct timeval val;
    gettimeofday(&val, NULL);
    struct tm* t = localtime(&val.tv_sec);

    log_memset_print(yyyymmdd, sizeof(yyyymmdd), "%04d%02d%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    log_memset_print(hhmmss, sizeof(hhmmss), "%02d%02d%02d", t->tm_hour, t->tm_min, t->tm_sec);
    log_memset_print(uuuuuu, sizeof(uuuuuu), "%06d", val.tv_usec);

    while (*_source != '\0') {
        if (strncmp(_source, LOG_MACRO_PREFIX, strlen(LOG_MACRO_PREFIX)) == 0) {
            if (strncmp(_source + strlen(LOG_MACRO_PREFIX), LOG_MACRO_YYYYMMDD, strlen(LOG_MACRO_YYYYMMDD)) == 0) {
                sprintf(_result + strlen(_result), "%s", yyyymmdd);
                _source += strlen(LOG_MACRO_PREFIX) + strlen(LOG_MACRO_YYYYMMDD);
            }
            else if (strncmp(_source + strlen(LOG_MACRO_PREFIX), LOG_MACRO_HHMMSS, strlen(LOG_MACRO_HHMMSS)) == 0) {
                sprintf(_result + strlen(_result), "%s", hhmmss);
                _source += strlen(LOG_MACRO_PREFIX) + strlen(LOG_MACRO_HHMMSS);
            }
            else if (strncmp(_source + strlen(LOG_MACRO_PREFIX), LOG_MACRO_UUUUUU, strlen(LOG_MACRO_UUUUUU)) == 0) {
                sprintf(_result + strlen(_result), "%s", uuuuuu);
                _source += strlen(LOG_MACRO_PREFIX) + strlen(LOG_MACRO_UUUUUU);
            }
            else if (strncmp(_source + strlen(LOG_MACRO_PREFIX), LOG_MACRO_TYPE, strlen(LOG_MACRO_TYPE)) == 0) {
                sprintf(_result + strlen(_result), "%s", _name);
                _source += strlen(LOG_MACRO_PREFIX) + strlen(LOG_MACRO_TYPE);
            }
            else if (strncmp(_source + strlen(LOG_MACRO_PREFIX), LOG_MACRO_ITH, strlen(LOG_MACRO_ITH)) == 0) {
                sprintf(_result + strlen(_result), "%02d", _ith);
                _source += strlen(LOG_MACRO_PREFIX) + strlen(LOG_MACRO_ITH);
            }
            else {
                return NULL;
            }
        }
        else {
            *(_result + strlen(_result)) = *_source++;
        }
    }

    *(_result + strlen(_result)) = '\0';

    return _result;
}

void make_directory_recursively(char* _filepath, size_t _size, mode_t _mode)
{
    if (_filepath == NULL) {
        return;
    }

    char dirpath[_size];
    memset(dirpath, 0x00, _size);
    snprintf(dirpath, _size, "%s", _filepath);
    for(char* p = dirpath + 1; *p; p++) {
        if(*p == '/') {
            *p = 0x00;
            mkdir(dirpath, _mode);
            *p = '/';
        }
    }
}


static void log_set_handler_file_variable(logger_handler* _handler, const char* _path0, const char* _directory_root, const char* _name, int _ith)
{
    if (_handler == NULL|| _path0 == NULL || _directory_root == NULL || _name == NULL) {
        return;
    }

    log_memset_print(_handler->u.file.path0, sizeof(_handler->u.file.path0), "%s", _path0);
    log_memset_print(_handler->u.file.directory_root, sizeof(_handler->u.file.directory_root), "%s", _directory_root);
    log_memset_print(_handler->u.file.name, sizeof(_handler->u.file.name), "%s", _name);
    _handler->u.file.ith = _ith;
}

static void log_update_handler_file_pass(logger_handler* _handler)
{
    if (_handler == NULL) {
        return;
    }

    char path1[LOG_SIZE_PATH_RELATIVE] = { 0 };
    modify_phrase_with_information(path1, _handler->u.file.path0, _handler->u.file.name, _handler->u.file.ith);
    log_memset_print(_handler->u.file.path, sizeof(_handler->u.file.path), "%s/%s", _handler->u.file.directory_root, path1);
    make_directory_recursively(_handler->u.file.path, sizeof(_handler->u.file.path), 0775);
}

static void log_update_handler_file_stream(logger_handler* _handler)
{
    if (_handler == NULL) {
        return;
    }

    FILE* fp = fopen(_handler->u.file.path, "rb");
    if (fp == NULL) {
        _handler->u.file.capacity_current = 0;
    }
    else {
        fseek(fp, 0, SEEK_END);
        _handler->u.file.capacity_current = ftell(fp);
        fclose(fp);
    }
    _handler->u.file.stream = fopen(_handler->u.file.path, "a");
}

static time_t log_get_time_rotation_next(logger_handler* _handler, time_t _time)
{
    if (_handler == NULL) {
        return 0;
    }

    int hour_next = -1;
    int minute_next = -1;
    int hour_min = 23;
    int minute_min = 59;
    struct tm* tm_time = localtime(&_time);
    JSON_array_item* item = _handler->u.file.items_time_rotation;
    if (item == NULL) {
        return 0;
    }

    while (item != NULL) {
        struct timeval time_rotation;
        JSON_value* value = json_array_item_get_value(item);
        JSON_array* array = json_value_get_array(value);
        int hour = (int)json_array_get_real_number(array, 0);
        int minute = (int)json_array_get_real_number(array, 1);
        if (tm_time->tm_hour < hour || (tm_time->tm_hour == hour && tm_time->tm_min < minute)) {
            if (hour_next < hour || (hour_next == hour && hour_next < minute)) {
                hour_next = hour;
                minute_next = minute;
            }
        }
        if (hour_min > hour || (hour_min == hour && minute_min > minute)) {
            hour_min = hour;
            minute_min = minute;
        }
        item = json_array_item_get_next(item);
    }

    if (hour_next < 0 || minute_next < 0) {
        hour_next = hour_min;
        minute_next = minute_next;
    }
    tm_time->tm_hour = hour_next;
    tm_time->tm_min = minute_next;
    tm_time->tm_sec = 0;
    time_t time_next = mktime(tm_time);
    if (hour_next < 0 || minute_next < 0) {
        time_next += 86400;
    }

    return time_next;
}

static int log_json_initialize_handler(JSON_object* _object_handlers, const char* _directory_root, const char* _name, int _ith)
{
    if (_object_handlers == NULL) {
        return LOG_NAK;
    }

    if (___mm_list_handler == NULL) {
        ___mm_list_handler = linked_list_create();
    }

    size_t length = json_object_get_length(_object_handlers);
    for (size_t ii = 0; ii < length; ++ii) {
        const char* name_handler = json_object_get_name(_object_handlers, ii);
        JSON_object* object_handler = json_object_get_object(_object_handlers, name_handler);
        const char* type = json_object_get_string(object_handler, LOG_NAME_TYPE);
        if (type == NULL) {
            log_json_finalize();
            return LOG_NAK;
        }
        if (strcmp(type, LOG_NAME_CONSOL) == 0) {
            logger_handler handler;
            memset(&handler, 0x00, sizeof(logger_handler));
            snprintf(handler.name, sizeof(handler.name), "%s", name_handler);
            const char* level = json_object_get_string(object_handler, LOG_NAME_LEVEL);
            if (level == NULL || (strcmp(level, LOG_VALUE_LEVEL_FATAL) != 0 && strcmp(level, LOG_VALUE_LEVEL_ERROR) != 0 && strcmp(level, LOG_VALUE_LEVEL_WARN) != 0 && strcmp(level, LOG_VALUE_LEVEL_INFO) != 0 && strcmp(level, LOG_VALUE_LEVEL_DEBUG) != 0 && strcmp(level, LOG_VALUE_LEVEL_TRACE) != 0)) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.level = log_convert_name_to_logger_level(level);
            handler.active = json_object_get_boolean(object_handler, LOG_NAME_ACTIVE);
            handler.type = lh_consol;

            const char* stream = json_object_get_string(object_handler, LOG_NAME_STREAM);
            if (stream == NULL || (strcmp(stream, LOG_VALUE_STREAM_STDOUT) != 0 && strcmp(stream, LOG_VALUE_STREAM_STDERR) != 0)) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.u.consol.stream = log_convert_name_to_stream(stream);

            linked_list_item* item = linked_list_item_create(&handler, sizeof(logger_handler));
            linked_list_append_item(___mm_list_handler, item);
        }
        else if (strcmp(type, LOG_NAME_FILE) == 0) {
            logger_handler handler;
            memset(&handler, 0x00, sizeof(logger_handler));
            snprintf(handler.name, sizeof(handler.name), "%s", name_handler);
            const char* level = json_object_get_string(object_handler, LOG_NAME_LEVEL);
            if (level == NULL || (strcmp(level, LOG_VALUE_LEVEL_FATAL) != 0 && strcmp(level, LOG_VALUE_LEVEL_ERROR) != 0 && strcmp(level, LOG_VALUE_LEVEL_WARN) != 0 && strcmp(level, LOG_VALUE_LEVEL_INFO) != 0 && strcmp(level, LOG_VALUE_LEVEL_DEBUG) != 0 && strcmp(level, LOG_VALUE_LEVEL_TRACE) != 0)) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.level = log_convert_name_to_logger_level(level);
            handler.active = json_object_get_boolean(object_handler, LOG_NAME_ACTIVE);
            handler.type = lh_file;

            const char* path = json_object_get_string(object_handler, LOG_NAME_PATH);
            if (path == NULL || strcmp(path, LOG_STRING_EMPTY) == 0) {
                log_json_finalize();
                return LOG_NAK;
            }
            log_set_handler_file_variable(&handler, path, _directory_root, _name, _ith);
            log_update_handler_file_pass(&handler);
            log_update_handler_file_stream(&handler);
            if (handler.u.file.stream == NULL) {
                log_json_finalize();
                return LOG_NAK;
            }
            // modify_phrase_with_information(path1, path0, _name, _ith);
            // log_memset_print(handler.u.file.path, sizeof(handler.u.file.path), "%s/%s", _directory_root, path1);
            // make_directory_recursively(handler.u.file.path, sizeof(handler.u.file.path), 0775);

            // FILE* fp = fopen(handler.u.file.path, "rb");
            // if (fp == NULL) {
            //     handler.u.file.capacity_current = 0;
            // }
            // else {
            //     fseek(fp, 0, SEEK_END);
            //     handler.u.file.capacity_current = ftell(fp);
            //     fclose(fp);
            // }
            // handler.u.file.stream = fopen(handler.u.file.path, "a");
            // if (handler.u.file.stream == NULL) {
            //     log_json_finalize();
            //     return LOG_NAK;
            // }

            const char* critical = json_object_get_string(object_handler, LOG_NAME_CRITICAL);
            if (critical == NULL || (strcmp(critical, LOG_VALUE_LEVEL_FATAL) != 0 && strcmp(critical, LOG_VALUE_LEVEL_ERROR) != 0 && strcmp(critical, LOG_VALUE_LEVEL_WARN) != 0 && strcmp(critical, LOG_VALUE_LEVEL_INFO) != 0 && strcmp(critical, LOG_VALUE_LEVEL_DEBUG) != 0 && strcmp(level, LOG_VALUE_LEVEL_TRACE) != 0)) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.u.file.critical = log_convert_name_to_logger_level(critical);

            JSON_value* value_time_rotation = json_value_duplicate(json_object_get_value(object_handler, LOG_NAME_TIME_ROTATION));
            JSON_array* array_time_rotation = json_value_get_array(value_time_rotation);
            handler.u.file.items_time_rotation = json_array_get_item_head(array_time_rotation);
            if (handler.u.file.items_time_rotation == NULL) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.u.file.time_rotation_next = log_get_time_rotation_next(&handler, time(NULL));

            const char* capacity_rotation = json_object_get_number_string(object_handler, LOG_NAME_CAPACITY_ROTATION);
            if (capacity_rotation == NULL || strcmp(capacity_rotation, LOG_STRING_EMPTY) == 0) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.u.file.capacity_rotation = strtol(capacity_rotation, NULL, 10);

            linked_list_item* item = linked_list_item_create(&handler, sizeof(logger_handler));
            linked_list_append_item(___mm_list_handler, item);
        }
        else if (strcmp(type, LOG_NAME_EMAIL) == 0) {
            logger_handler handler;
            memset(&handler, 0x00, sizeof(logger_handler));
            snprintf(handler.name, sizeof(handler.name), "%s", name_handler);
            const char* level = json_object_get_string(object_handler, LOG_NAME_LEVEL);
            if (level == NULL || (strcmp(level, LOG_VALUE_LEVEL_FATAL) != 0 && strcmp(level, LOG_VALUE_LEVEL_ERROR) != 0 && strcmp(level, LOG_VALUE_LEVEL_WARN) != 0 && strcmp(level, LOG_VALUE_LEVEL_INFO) != 0 && strcmp(level, LOG_VALUE_LEVEL_DEBUG) != 0 && strcmp(level, LOG_VALUE_LEVEL_TRACE) != 0)) {
                log_json_finalize();
                return LOG_NAK;
            }
            handler.level = log_convert_name_to_logger_level(level);
            handler.active = json_object_get_boolean(object_handler, LOG_NAME_ACTIVE);
            handler.type = lh_email;

            JSON_array* receivers = json_object_get_array(object_handler, LOG_NAME_RECEIVERS);
            handler.u.email.length = json_array_get_length(receivers);
            handler.u.email.receivers = (char**)malloc(sizeof(char*) * handler.u.email.length);
            for (size_t ii = 0; ii < handler.u.email.length; ++ii) {
                const char* receiver = json_array_get_string(receivers, ii);
                *(handler.u.email.receivers + ii) = (char*)malloc(strlen(receiver) + 1);
                log_memset_print(*(handler.u.email.receivers + ii), strlen(receiver) + 1, "%s", receiver);
            }

            linked_list_item* item = linked_list_item_create(&handler, sizeof(logger_handler));
            linked_list_append_item(___mm_list_handler, item);
        }
        else {
            log_json_finalize();
            return LOG_NAK;
        }
    }
}

static int log_json_initialize_logger(JSON_object* _object_loggers, JSON_object* _object_handlers, const char* _fulltag_parent, size_t _depth)
{
    if (_object_loggers == NULL || _object_handlers == NULL) {
        log_json_finalize();
        return LOG_NAK;
    }

    size_t length_object_loggers = json_object_get_length(_object_loggers);
    for (size_t ii = 0; ii < length_object_loggers; ++ii) {
        JSON_object* object_logger = json_object_get_ith_object(_object_loggers, ii);
        json_object_set_array(object_logger, LOG_NAME_HANDLERS_POINTER);

        const char* tag = json_object_get_string(object_logger, LOG_NAME_TAG);
        for (size_t jj = 0; jj < strlen(tag); ++jj) {
            if ((*(tag + jj) < 'A') || (*(tag + jj) > 'Z') && (*(tag + jj) < 'a') || (*(tag + jj) > 'z')) {
                log_json_finalize();
                return LOG_NAK;
            }
        }

        char fulltag[LOG_SIZE_FULLTAG];
        if (_fulltag_parent != NULL && strcmp(_fulltag_parent, LOG_STRING_EMPTY) != 0) {
            log_memset_print(fulltag, sizeof(fulltag), "%s.%s", _fulltag_parent, tag);
        }
        else {
            log_memset_print(fulltag, sizeof(fulltag), "%s", tag);
        }
        json_object_set_string(object_logger, LOG_NAME_FULLTAG, fulltag);

        JSON_array* handlers_pointer = json_object_get_array(object_logger, LOG_NAME_HANDLERS_POINTER);
        size_t length_object_handlers = json_object_get_length(_object_handlers);
        JSON_array* array_handlers = json_object_get_array(object_logger, LOG_NAME_HANDLERS);
        if (array_handlers != NULL) {
            size_t length_array_handlers = json_array_get_length(array_handlers);
            for (size_t jj = 0; jj < length_array_handlers; ++jj) {
                const char* name_array_handler = json_array_get_string(array_handlers, jj);
                if (name_array_handler == NULL || strcmp(name_array_handler, LOG_STRING_EMPTY) == 0) {
                    log_json_finalize();
                    return LOG_NAK;
                }
                if (json_object_has_name(_object_handlers, name_array_handler) == false)  {
                    log_json_finalize();
                    return LOG_NAK;
                }

                bool found = false;
                linked_list_item* item = linked_list_get_head(___mm_list_handler);
                while (item != NULL) {
                    logger_handler* handler = (logger_handler*)linked_list_item_get_data(item);
                    if (strcmp(name_array_handler, handler->name) == 0) {
                        found = true;
                        double data_double_handler = log_convert_pointer_to_double((void*)handler);
                        json_array_append_number(handlers_pointer, data_double_handler);
                        break;
                    }
                    item = linked_list_item_get_next(item);
                }
                if (found == false) {
                    log_json_finalize();
                    return LOG_NAK;
                }
            }
        }

        JSON_object* object_loggers_sub = json_object_get_object(object_logger, LOG_NAME_LOGGERS);
        if (object_loggers_sub != NULL) {
            log_json_initialize_logger(object_loggers_sub, _object_handlers, fulltag, _depth + 1);
        }
    }

    return LOG_ACK;
}

JSON_value* log_json_intialize(const char* _directory_root, const char* _filepath_log, const char* _type, int _ith)
{
    if (_directory_root == NULL || _filepath_log == NULL) {
        return NULL;
    }

    char path[LOG_SIZE_PATH] = { 0 };
    log_memset_print(path, sizeof(path), "%s/%s", _directory_root, _filepath_log);
    ___mm_value_logger = json_parse_file(path);
    if (___mm_value_logger == NULL) {
        return NULL;
    }
    JSON_object* mm_object_logger = json_value_get_object(___mm_value_logger);

    const char* name_sem0 = json_object_get_string(mm_object_logger, LOG_NAME_NAME_SEM);
    if (name_sem0 == NULL || strcmp(name_sem0, LOG_STRING_EMPTY) == 0 || *name_sem0 != '/') {
        ___mm_lock_logger = logger_lock_mutex;
        ___mm_unlock_logger = logger_unlock_mutex;
    }
    else {
        ___mm_lock_logger = logger_lock_semaphore;
        ___mm_unlock_logger = logger_unlock_semaphore;
        char name_sem1[LOG_SIZE_NAME_SEM] = { 0 };
        modify_phrase_with_information(name_sem1, name_sem0, _type, _ith);
        log_memset_print(___mm_name_sem, sizeof(___mm_name_sem), "%s", name_sem1);
        sem_unlink(name_sem1);
        if ((___mm_sem_logger = sem_open(name_sem1, O_CREAT, 0644, 1)) == NULL) {
            log_json_finalize();
            return NULL;
        }
    }

    const char* label0 = json_object_get_string(mm_object_logger, LOG_NAME_LABEL);
    if (label0 != NULL && strcmp(label0, LOG_STRING_EMPTY) != 0) {
        char label1[LOG_SIZE_LABEL] = { 0 };
        modify_phrase_with_information(label1, label0, _type, _ith);
        log_memset_print(___mm_label_logger, sizeof(___mm_label_logger), "%s", label1);
    }

    JSON_object* object_handlers = json_object_get_object(mm_object_logger, LOG_NAME_HANDLERS);
    if (object_handlers == NULL) {
        log_json_finalize();
        return NULL;
    }
    if (json_object_get_length(object_handlers) == 0) {
        log_json_finalize();
        return NULL;
    }
    if (log_json_initialize_handler(object_handlers, _directory_root, _type, _ith) < LOG_ACK) {
        log_json_finalize();
        return NULL;
    }
    JSON_object* object_loggers = json_object_get_object(mm_object_logger, LOG_NAME_LOGGERS);
    if (object_loggers == NULL) {
        log_json_finalize();
        return NULL;
    }
    // if (json_object_get_length(object_loggers) != 1) {
    //     log_json_finalize();
    //     return NULL;
    // }
    if (strcmp(json_object_get_name(object_loggers, 0), LOG_NAME_ROOT) != 0) {
        log_json_finalize();
        return NULL;
    }
    log_json_initialize_logger(object_loggers, object_handlers, NULL, 0);

    return ___mm_value_logger;
}

static bool log_destroy_handler_item(linked_list_item* _item, void* _arg)
{
    logger_handler* handler = (logger_handler*)linked_list_item_get_data(_item);
    if (handler->type == lh_email) {
        for (size_t ii = 0; ii < handler->u.email.length; ++ii) {
            free(*(handler->u.email.receivers + ii));
        }
        free(handler->u.email.receivers);
    }
}

void log_json_finalize()
{
    log_fflush();
    if (___mm_value_logger != NULL) {
        json_value_free(___mm_value_logger);
    }
    if (___mm_list_handler != NULL) {
        linked_list_destroy(___mm_list_handler, log_destroy_handler_item, NULL);
    }
    if (___mm_sem_logger != NULL) {
        sem_close(___mm_sem_logger);
    }
}


static void log_write_process_handlers(JSON_object* _logger, struct timeval _logtime, logger_level _level, const char* _fulltag, pid_t _pid, pthread_t _tid, const char* _filepath, const char* _function, int _line, const char* _message)
{
    if (_logger == NULL || _filepath == NULL || _message == NULL) {
        return;
    }

    if (log_convert_name_to_logger_level(json_object_get_string(_logger, LOG_NAME_LEVEL)) <= _level) {
        JSON_array* array_handlers = json_object_get_array(_logger, LOG_NAME_HANDLERS_POINTER);
        if (array_handlers != NULL) {
            size_t length_handlers = json_array_get_length(array_handlers);
            char logtime[LOG_SIZE_TIME] = { 0, };
            logger_write_time(logtime, _logtime);
            char loglevel[LOG_SIZE_LEVEL] = { 0, };
            logger_write_level(loglevel, _level);
            for (size_t ii = 0; ii < length_handlers; ++ii) {
                logger_handler* handler = (logger_handler*)log_convert_double_to_pointer(json_array_get_real_number(array_handlers, ii));
                if (handler->level <= _level) {
                    if (handler->type == lh_consol) {
                        if (___mm_label_logger == NULL) {
                            fprintf(handler->u.consol.stream, "%s [%s] [%s] %s (%d:%lu) (%s:%s:%d)\n", logtime, loglevel, _fulltag, _message, _pid, _tid, _filepath, _function, _line);
                        }
                        else {
                            fprintf(handler->u.consol.stream, "%s [%s] [%s] [%s] %s (%d:%lu) (%s:%s:%d)\n", logtime, loglevel, ___mm_label_logger, _fulltag, _message, _pid, _tid, _filepath, _function, _line);
                        }
                    }
                    else if (handler->type == lh_file) {
                        if ((handler->u.file.time_rotation_next != 0 && handler->u.file.time_rotation_next <= _logtime.tv_sec) || (handler->u.file.capacity_rotation != 0 && handler->u.file.capacity_current >= handler->u.file.capacity_rotation)) {
                            log_update_handler_file_pass(handler);
                            if (___mm_label_logger == NULL) {
                                fprintf(handler->u.file.stream, "%s [%s] [%s] %s(%s) (%d:%lu) (%s:%s:%d)\n", logtime, LOG_SYSTEM_LEVEL_NOTHING, LOG_SYSTEM_FULLTAG_NOTHING, LOG_SYSTEM_MESSAGE_ROTATE_FILE, handler->u.file.path, _pid, _tid, _filepath, _function, _line);
                            }
                            else {
                                fprintf(handler->u.file.stream, "%s [%s] [%s] [%s] %s(%s) (%d:%lu) (%s:%s:%d)\n", logtime, LOG_SYSTEM_LEVEL_NOTHING, ___mm_label_logger, LOG_SYSTEM_FULLTAG_NOTHING, LOG_SYSTEM_MESSAGE_ROTATE_FILE, handler->u.file.path, _pid, _tid, _filepath, _function, _line);
                            }
                            fflush(handler->u.file.stream);
                            fclose(handler->u.file.stream);
                            handler->u.file.stream = NULL;

                            log_update_handler_file_stream(handler);
                            if (handler->u.file.time_rotation_next != 0 && handler->u.file.time_rotation_next <= _logtime.tv_sec) {
                                handler->u.file.time_rotation_next = log_get_time_rotation_next(handler, time(NULL));
                            }
                        }

                        int size = 0;
                        if (___mm_label_logger == NULL) {
                            size = fprintf(handler->u.file.stream, "%s [%s] [%s] %s (%d:%lu) (%s:%s:%d)\n", logtime, loglevel, _fulltag, _message, _pid, _tid, _filepath, _function, _line);
                        }
                        else {
                            size = fprintf(handler->u.file.stream, "%s [%s] [%s] [%s] %s (%d:%lu) (%s:%s:%d)\n", logtime, loglevel, ___mm_label_logger, _fulltag, _message, _pid, _tid, _filepath, _function, _line);
                        }
                        handler->u.file.capacity_current += size;
                        if (handler->u.file.critical <= _level) {
                            fflush(handler->u.file.stream);
                        }
                    }
                    else if (handler->type == lh_email) {
                        const char* subject = LOG_MAIL_SUBJECT;

                        char** content = (char**)malloc(sizeof(char*) * 7);

                        char content_logtime[6 + LOG_SIZE_TIME + 2];
                        log_memset_print(content_logtime, sizeof(content_logtime), "TIME: %s\n", logtime);
                        *(content + 0) = content_logtime;

                        char content_loglevel[7 + LOG_SIZE_LEVEL + 2];
                        log_memset_print(content_loglevel, sizeof(content_loglevel), "LEVEL: %s\n", loglevel);
                        *(content + 1) = content_loglevel;

                        char content_loglabel[7 + LOG_SIZE_LABEL + 2];
                        if (___mm_label_logger == NULL) {
                            log_memset_print(content_loglabel, sizeof(content_loglabel), LOG_STRING_EMPTY);
                        }
                        else {
                            log_memset_print(content_loglabel, sizeof(content_loglabel), "LABEL: %s\n", ___mm_label_logger);
                        }
                        *(content + 2) = content_loglabel;

                        char content_fulltag[5 + LOG_SIZE_FULLTAG + 2];
                        log_memset_print(content_fulltag, sizeof(content_fulltag), "TAG: %s\n", _fulltag);
                        *(content + 3) = content_fulltag;

                        char content_content[9 + strlen(_message) + 2];
                        log_memset_print(content_content, sizeof(content_content), "CONTENT: %s\n", _message);
                        *(content + 4) = content_content;

                        char content_pid_tid[10 + LOG_SIZE_LONG + 2 + LOG_SIZE_LONG + 2];
                        log_memset_print(content_pid_tid, sizeof(content_pid_tid), "PID, TID: %d, %lu\n", _pid, _tid);
                        *(content + 5) = content_pid_tid;

                        char content_file_line[12 + LOG_SIZE_PATH + 2 + LOG_SIZE_LONG + 2];
                        log_memset_print(content_file_line, sizeof(content_file_line), "FILE, FUNCTION, LINE: %s, %s, %d\n", _filepath, _function, _line);
                        *(content + 6) = content_file_line;

                        char buffer_tls_receive[8 + handler->u.email.length + 3][MAIL_SIZE_SSL_READ];
                        mail_tls_send(buffer_tls_receive, MAIL_SENDER, handler->u.email.receivers, handler->u.email.length, subject, content, 7);
                        // char buffer_ssl_receive[6 + handler->u.email.length + 3][MAIL_SIZE_SSL_READ];
                        // mail_ssl_send(buffer_ssl_receive, MAIL_SENDER, handler->u.email.receivers, handler->u.email.length, subject, content, 7);

                        free(content);
                    }
                }
            }
        }
    }

    bool propagate = json_object_get_boolean(_logger, LOG_NAME_PROPAGATE);
    if (propagate == true) {
        JSON_value* value_logger = json_object_get_value_wrapper(_logger);
        JSON_value* value_parent = json_value_get_parent(json_value_get_parent(value_logger));
        JSON_object* object_parent = json_value_get_object(value_parent);
        if (object_parent != NULL) {
            log_write_process_handlers(object_parent, _logtime, _level, _fulltag, _pid, _tid, _filepath, _function, _line, _message);
        }
    }
}

void log_write(JSON_object* _logger, logger_level _level, pid_t _pid, pthread_t _tid, const char* _filepath, const char* _function, int _line, const char* _message, ...)
{
    if (_logger == NULL || _filepath == NULL || _message == NULL) {
        return;
    }

    ___mm_lock_logger();

    struct timeval now;
    gettimeofday(&now, NULL);

    size_t size = (size_t)json_object_get_real_number(_logger, LOG_NAME_SIZE);
    char message[size];
    memset(message, 0x00, size);
    va_list argp;
    va_start(argp, _message);
    vsnprintf(message, size, _message, argp);
    va_end(argp);

    const char* fulltag = json_object_get_string(_logger, LOG_NAME_FULLTAG);
    log_write_process_handlers(_logger, now, _level, fulltag, _pid, _tid, _filepath, _function, _line, message);

    ___mm_unlock_logger();
}

void log_fflush()
{
    if (___mm_list_handler == NULL) {
        return;
    }
    linked_list_item* item = linked_list_get_head(___mm_list_handler);
    while (item != NULL) {
        logger_handler* handler = (logger_handler*)linked_list_item_get_data(item);
        if (handler->type == lh_file) {
            fflush(handler->u.file.stream);
        }
        item = linked_list_item_get_next(item);
    }
}
