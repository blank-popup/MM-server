#ifndef ___MM_KIT_LOG_H___
#define ___MM_KIT_LOG_H___

#include <stdio.h>
#include <stdbool.h>
#include "kit/json/mmJson.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LOG_SIZE_NAME_SEM (64)
#define LOG_SIZE_LABEL (64)
#define LOG_SIZE_NAME_HANDLER (64)
#define LOG_SIZE_NAME_PROCESS (64)

#define LOG_SIZE_PATH (1024)
#define LOG_SIZE_PATH_RELATIVE (256)
#define LOG_SIZE_SMTP_MAIL (256)

#define LOG_SIZE_TIME (27)
#define LOG_SIZE_LEVEL (12)
#define LOG_SIZE_TAG (64)
#define LOG_DEPTH_TAG (32)
#define LOG_SIZE_FULLTAG (LOG_SIZE_TAG * LOG_DEPTH_TAG)
#define LOG_SIZE_LONG (18)

#define LOG_SIZE_YYYYMMDD (9)
#define LOG_SIZE_HHMMSS (7)
#define LOG_SIZE_UUUUUU (7)
#define LOG_SIZE_NUMBER (3)
#define LOG_MACRO_PREFIX "?"
#define LOG_MACRO_YYYYMMDD "yyyymmdd"
#define LOG_MACRO_HHMMSS "hhmmss"
#define LOG_MACRO_UUUUUU "uuuuuu"
#define LOG_MACRO_TYPE "type"
#define LOG_MACRO_ITH "##"

#define LOG_VALUE_LEVEL_FATAL "fatal"
#define LOG_VALUE_LEVEL_ERROR "error"
#define LOG_VALUE_LEVEL_WARN "warn"
#define LOG_VALUE_LEVEL_INFO "info"
#define LOG_VALUE_LEVEL_DEBUG "debug"
#define LOG_VALUE_LEVEL_TRACE "trace"

#define LOG_SYSTEM_LEVEL_NOTHING "="
#define LOG_SYSTEM_FULLTAG_NOTHING "==="
#define LOG_SYSTEM_MESSAGE_ROTATE_FILE "Rotated log file"

#define LOG_VALUE_STREAM_STDOUT "stdout"
#define LOG_VALUE_STREAM_STDERR "stderr"

#define LOG_NAME_NAME_SEM "name_sem"
#define LOG_NAME_LABEL "label"
#define LOG_NAME_LOGGERS "loggers"
#define LOG_NAME_ROOT "root"
#define LOG_NAME_HANDLERS_POINTER "handlers_pointer"
#define LOG_NAME_TAG "tag"
#define LOG_NAME_FULLTAG "fulltag"
#define LOG_NAME_LEVEL "level"
#define LOG_NAME_SIZE "size"
#define LOG_NAME_ACTIVE "active"
#define LOG_NAME_PROPAGATE "propagate"
#define LOG_NAME_HANDLERS "handlers"
#define LOG_NAME_TYPE "type"
#define LOG_NAME_CONSOL "consol"
#define LOG_NAME_STREAM "stream"
#define LOG_NAME_FILE "file"
#define LOG_NAME_PATH "path"
#define LOG_NAME_CRITICAL "critical"
#define LOG_NAME_TIME_ROTATION "time_rotation"
#define LOG_NAME_CAPACITY_ROTATION "capacity_rotation"
#define LOG_NAME_EMAIL "email"
#define LOG_NAME_RECEIVERS "receivers"

#define LOG_STRING_EMPTY ""
#define LOG_MAIL_SUBJECT "MM IMPORTANT LOG"

#define LOG_ACK (0)
#define LOG_NAK (-1)

    typedef enum _logger_level {
        ll_fatal = 60,
        ll_error = 50,
        ll_warn = 40,
        ll_info = 30,
        ll_debug = 20,
        ll_trace = 10
    } logger_level;

    typedef enum _logger_handler_type {
        lh_consol,
        lh_file,
        lh_email
    } logger_handler_type;

    typedef struct _logger logger;

    char* log_memset_print(char* _buffer, size_t _size, const char* _message, ...);
    void make_directory_recursively(char* _filepath, size_t _size, mode_t _mode);

    JSON_value* log_json_intialize(const char* _directory_root, const char* _filepath_log, const char* _type, int _ith);
    void log_json_finalize();
    void log_write(JSON_object* _logger, logger_level _level, pid_t _pid, pthread_t _tid, const char* _filepath, const char* _function, int _line, const char* _message, ...);
    void log_fflush();

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define mmlog(l, ll, fmt, ...) log_write(l, ll, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define ltrace(l, fmt, ...) log_write(l, ll_trace, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define ldebug(l, fmt, ...) log_write(l, ll_debug, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define linfo(l, fmt, ...) log_write(l, ll_info, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define lwarn(l, fmt, ...) log_write(l, ll_warn, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define lerror(l, fmt, ...) log_write(l, ll_error, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define lfatal(l, fmt, ...) log_write(l, ll_fatal, getpid(), pthread_self(), __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_LOG_H___
