#ifndef ___MM_LOG_SIMPLE_H___
#define ___MM_LOG_SIMPLE_H___

#include <unistd.h>
#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif


#define MM_SIZE_PATH_LOG_SIMPLE (1024)
#define MM_DIRECTORY_LOG_SIMPLE "LOG_mmm/"
#define MM_SIZE_LINE_LOG_SIMPLE (4096)

    void LogSimple(int _level, const char* _filepath, int _line, pid_t _pid, pthread_t _tid, const char* _message, ...);
#define mmmlog(ll, fmt, ...) LogSimple(ll, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)
#define lltrace(fmt, ...) LogSimple(10, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)
#define lldebug(fmt, ...) LogSimple(20, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)
#define llinfo(fmt, ...) LogSimple(30, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)
#define llwarn(fmt, ...) LogSimple(40, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)
#define llerror(fmt, ...) LogSimple(50, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)
#define llfatal(fmt, ...) LogSimple(60, __FILE__, __LINE__, getpid(), pthread_self(), fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // ___MM_LOG_SIMPLE_H___
