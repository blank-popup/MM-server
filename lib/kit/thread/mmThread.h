#ifndef ___MM_KIT_THREAD_H___
#define ___MM_KIT_THREAD_H___

#include <stdbool.h>
#include <stddef.h>
#include "kit/structure/mmLinkedList.h"


#ifdef __cplusplus
extern "C" {
#endif

    #define STATUS_MIN_THREAD (1)
    #define STATUS_MAX_THREAD (10)
    #define DIFFERENCE_STATUS (STATUS_MAX_THREAD - STATUS_MIN_THREAD)

    #define STATUS_THREAD_START STATUS_MIN_THREAD
    #define STATUS_THREAD_WAIT_EPOLL (STATUS_THREAD_START + 1)
    #define STATUS_THREAD_RUN_TASK (STATUS_THREAD_START + 2)

    typedef struct _thread_task thread_task;
    typedef struct _threadpool_thread threadpool_thread;
    typedef struct _threadpool threadpool;
    typedef void*(*action_task)(void*);
    typedef void(*action_pool)(void);

    linked_list* tpool_tasks_create(size_t _count);
    void tpool_tasks_destroy(linked_list* _tasks);
    thread_task* tpool_tasks_get_task(linked_list* _tasks, size_t _ith);

    void tpool_task_set_action(thread_task* _task, action_task _action, action_task _pre, action_task _post);
    void tpool_task_set_arg(thread_task* _task, void* _arg, void(*_arg_destroy)(void*));

    linked_list* tpool_threadpool_get_threads(threadpool* _pool);
    threadpool_thread* tpool_threadpool_get_thread(threadpool* _pool, pthread_t _pthread);
    threadpool_thread* tpool_threadpool_get_thread_ith(threadpool* _pool, size_t _ith);
    threadpool_thread* tpool_threadpool_get_thread_self(threadpool* _pool);
    pthread_t tpool_thread_get_pthread(threadpool_thread* _thread);
    thread_task* tpool_thread_get_task(threadpool_thread* _thread);
    void* tpool_thread_get_option(threadpool_thread* _thread);
    void* tpool_task_get_arg(thread_task* _task);
    threadpool_thread* tpool_task_get_thread(thread_task* _task);
    void tpool_task_set_expire(thread_task* _task, struct timeval _timeval);
    struct timeval tpool_task_get_expire(thread_task* _task);
    threadpool_thread* tpool_threadpool_set_self_thread_status(threadpool* _pool, int _status);
    int tpool_threadpool_get_self_thread_status(threadpool* _pool);

    threadpool* tpool_threadpool_create(int _status_min_thread, int _status_max_thread, action_pool _pre, action_pool _post, void* _option, void(*_option_destroy)(void*));
    void tpool_threadpool_destroy(threadpool* _pool);

    void tpool_threadpool_add_thread(threadpool* _pool, void* _option, void(*_option_destroy)(void*));
    void tpool_threadpool_remove_thread(threadpool* _pool);

    bool tpool_threadpool_add_thread_task(threadpool* _pool, thread_task* _task);
    // bool tpool_threadpool_add_thread_task(threadpool* _pool, action_task _action, void* _arg, void(*_arg_destroy)(void*), action_task _pre, action_task _post);
    void tpool_threadpool_wait_no_task(threadpool *_pool);

    void* tpool_threadpool_get_thread_task_arg(threadpool* _pool, size_t _ith);

    bool tpool_check_thread_timeout(thread_task* _task, struct timeval _time_current);
    void tpool_restart_thread(threadpool* _pool, thread_task* _task);
    int tpool_check_restart_threads_timeout(threadpool* _pool);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_THREAD_H___
