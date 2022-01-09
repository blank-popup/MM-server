#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include "mmThread.h"
#include "kit/structure/mmQueue.h"
#include "../../mmLogSimple.h"


struct _thread_task {
    action_task pre;
    action_task action;
    action_task post;
    void* arg;
    void(*arg_destroy)(void*);
    struct timeval expire;
    threadpool_thread* thread;
};

struct _threadpool_thread {
    pthread_t pthread;
    thread_task* task;
    int status;
    void* option;
    void(*option_destroy)(void*);
};

struct _threadpool {
    queue* tasks_waiting;
    linked_list* threads;
    pthread_mutex_t lock;
    pthread_cond_t notify_task;
    pthread_cond_t notify_no_task;
    int status_min_thread;
    int status_max_thread;
    bool terminate;
    void* option;
    void(*option_destroy)(void*);
    action_pool pre;
    action_pool post;
};


linked_list* tpool_tasks_create(size_t _count)
{
    linked_list* tasks = linked_list_create();
    for (size_t ii = 0; ii < _count; ++ii) {
        thread_task task = { 0 };
        memset(&task, 0x00, sizeof(thread_task));
        linked_list_item* item = linked_list_item_create(&task, sizeof(thread_task));
        linked_list_append_item(tasks, item);
    }

    return tasks;
}

void tpool_tasks_destroy(linked_list* _tasks)
{
    linked_list_destroy(_tasks, NULL, NULL);
}

thread_task* tpool_tasks_get_task(linked_list* _tasks, size_t _ith)
{
    if (_tasks == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_ith(_tasks, _ith);
    thread_task* task = linked_list_item_get_data(item);

    return task;
}

void tpool_task_set_action(thread_task* _task, action_task _action, action_task _pre, action_task _post)
{
    if (_task == NULL) {
        return;
    }
    _task->pre = _pre;
    _task->action = _action;
    _task->post = _post;
}

void tpool_task_set_arg(thread_task* _task, void* _arg, void(*_arg_destroy)(void*))
{
    if (_task == NULL) {
        return;
    }
    _task->arg = _arg;
    _task->arg_destroy = _arg_destroy;
}


// static queue_item* tpool_queue_item_task_create(action_task _action, void* _arg, void(*_arg_destroy)(void*), action_task _pre, action_task _post)
// {
//     if (_action == NULL) {
//         return NULL;
//     }

//     thread_task task = { .pre = _pre, .action = _action, .post = _post, .arg = _arg, .arg_destroy = _arg_destroy };
//     queue_item* item = queue_item_create(&task, sizeof(thread_task));

//     return item;
// }

// static void tpool_queue_item_task_destroy(queue_item* _item)
// {
//     if (_item == NULL) {
//         return;
//     }

//     queue_item_destroy(_item, NULL, NULL);
// }

// static queue_item* tpool_queue_item_task_pop(threadpool* _pool)
// {
//     if (_pool == NULL) {
//         return NULL;
//     }

//     queue_item* item = queue_get_item(_pool->tasks_waiting);
//     return item;
// }

static size_t tpool_threadpool_get_count_status_thread(threadpool* _pool, int _status_min_thread, int _status_max_thread)
{
    if (_pool == NULL) {
        return 0;
    }

    linked_list_item* item = linked_list_get_head(_pool->threads);
    size_t count = 0;
    while (item != NULL) {
        threadpool_thread* thread = linked_list_item_get_data(item);
        if (thread->status >= _status_min_thread && thread->status <= _status_max_thread) {
            ++count;
        }
        item = linked_list_item_get_next(item);
    }

    return count;
}

linked_list* tpool_threadpool_get_threads(threadpool* _pool)
{
    if (_pool == NULL) {
        return NULL;
    }

    return _pool->threads;
}

threadpool_thread* tpool_threadpool_get_thread(threadpool* _pool, pthread_t _pthread)
{
    if (_pool == NULL || _pool->threads == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_head(_pool->threads);
    while (item != NULL) {
        threadpool_thread* thread = linked_list_item_get_data(item);
        if (thread != NULL && thread->pthread == _pthread) {
            return thread;
        }
        item = linked_list_item_get_next(item);
    }

    return NULL;
}

threadpool_thread* tpool_threadpool_get_thread_ith(threadpool* _pool, size_t _ith)
{
    if (_pool == NULL || _pool->threads == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_head(_pool->threads);
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (item == NULL) {
            return NULL;
        }
        item = linked_list_item_get_next(item);
    }

    if (item != NULL) {
        return linked_list_item_get_data(item);
    }

    return NULL;
}

threadpool_thread* tpool_threadpool_get_thread_self(threadpool* _pool)
{
    return tpool_threadpool_get_thread(_pool, pthread_self());
}

pthread_t tpool_thread_get_pthread(threadpool_thread* _thread)
{
    if (_thread == NULL) {
        return 0;
    }

    return _thread->pthread;
}

thread_task* tpool_thread_get_task(threadpool_thread* _thread)
{
    if (_thread == NULL) {
        return NULL;
    }

    return _thread->task;
}

void* tpool_thread_get_option(threadpool_thread* _thread)
{
    if (_thread == NULL) {
        return NULL;
    }

    return _thread->option;
}

void* tpool_task_get_arg(thread_task* _task)
{
    if (_task == NULL) {
        return NULL;
    }

    return _task->arg;
}

threadpool_thread* tpool_task_get_thread(thread_task* _task)
{
    if (_task == NULL) {
        return NULL;
    }

    return _task->thread;
}

void tpool_task_set_expire(thread_task* _task, struct timeval _timeval)
{
    if (_task == NULL) {
        return;
    }

    _task->expire = _timeval;
}

struct timeval tpool_task_get_expire(thread_task* _task)
{
    if (_task == NULL) {
        return (struct timeval){0, 0};
    }

    return _task->expire;
}

threadpool_thread* tpool_threadpool_set_self_thread_status(threadpool* _pool, int _status)
{
    threadpool_thread* thread = tpool_threadpool_get_thread_self(_pool);
    if (thread != NULL) {
        thread->status = _status;
        return thread;
    }

    return NULL;
}

int tpool_threadpool_get_self_thread_status(threadpool* _pool)
{
    threadpool_thread* thread = tpool_threadpool_get_thread_self(_pool);
    if (thread != NULL) {
        return thread->status;
    }

    return -1;
}

// static threadpool_thread* tpool_threadpool_set_self_thread_task(threadpool* _pool, thread_task* _task)
// {
//     threadpool_thread* thread = tpool_threadpool_get_thread_self(_pool);
//     if (thread != NULL) {
//         thread->task = _task;
//         return thread;
//     }

//     return NULL;
// }

// static threadpool_thread* tpool_threadpool_set_self_task_thread(threadpool* _pool, thread_task* _task)
// {
//     threadpool_thread* thread = tpool_threadpool_get_thread_self(_pool);
//     if (thread != NULL) {
//         _task->thread = thread;
//         return thread;
//     }

//     return NULL;
// }

static void* tpool_thread_tasker(void* _arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    threadpool* pool = _arg;

    if (pool->pre != NULL) {
        pool->pre();
    }

    while (true) {
        pthread_mutex_lock(&pool->lock);

        while ((pool->tasks_waiting == NULL || queue_get_count(pool->tasks_waiting) == 0) && !pool->terminate) {
            pthread_cond_wait(&(pool->notify_task), &(pool->lock));
        }

        if (pool->terminate) {
            pthread_mutex_unlock(&pool->lock);
            break;
        }

        queue_item* item = queue_get_item(pool->tasks_waiting);
        // queue_item* item = tpool_queue_item_task_pop(pool);
        thread_task* task = *(thread_task**)queue_item_get_data(item);
        pthread_mutex_unlock(&pool->lock);

        if (task != NULL) {
            tpool_threadpool_set_self_thread_status(pool, pool->status_min_thread + 1);
            threadpool_thread* thread = tpool_threadpool_get_thread_self(pool);
            thread->task = task;
            task->thread = thread;
            // tpool_threadpool_set_self_thread_task(pool, task);
            // tpool_threadpool_set_self_task_thread(pool, task);
            if (task->pre != NULL) {
                task->pre(pool);
            }
            task->action(task);
            // task->action(task->arg);
            if (task->post != NULL) {
                task->post(pool);
            }
            if (task->arg_destroy != NULL) {
                task->arg_destroy(task->arg);
            }
        }

        tpool_threadpool_set_self_thread_status(pool, pool->status_min_thread);
        queue_item_destroy(item, NULL, NULL);
        // tpool_queue_item_task_destroy(item);

        pthread_mutex_lock(&pool->lock);

        if ((tpool_threadpool_get_count_status_thread(pool, pool->status_min_thread + 1, pool->status_max_thread) == 0) && queue_get_count(pool->tasks_waiting) == 0) {
            pthread_cond_signal(&(pool->notify_no_task));
        }

        pthread_mutex_unlock(&pool->lock);
    }

    // pthread_mutex_unlock(&pool->lock);

    if (pool->post != NULL) {
        pool->post();
    }

    return NULL;
}

threadpool* tpool_threadpool_create(int _status_min_thread, int _status_max_thread, action_pool _pre, action_pool _post, void* _option, void(*_option_destroy)(void*))
{
    if (_status_max_thread - _status_min_thread < DIFFERENCE_STATUS) {
        return NULL;
    }

    threadpool* pool = (threadpool*)malloc(sizeof(threadpool));
    memset(pool, 0x00, sizeof(threadpool));

    pthread_mutex_init(&(pool->lock), NULL);
    pthread_cond_init(&(pool->notify_task), NULL);
    pthread_cond_init(&(pool->notify_no_task), NULL);

    pool->tasks_waiting = queue_create();
    pool->threads = linked_list_create();
    pool->status_min_thread = _status_min_thread;
    pool->status_max_thread = _status_max_thread;
    pool->terminate = false;
    pool->option = _option;
    pool->option_destroy = _option_destroy;
    pool->pre = _pre;
    pool->post = _post;

    return pool;
}

void tpool_threadpool_destroy(threadpool* _pool)
{
    if (_pool == NULL) {
        return;
    }

    pthread_mutex_lock(&_pool->lock);
    queue_destroy(_pool->tasks_waiting, NULL, NULL);
    _pool->terminate = true;
    pthread_cond_broadcast(&_pool->notify_task);
    pthread_mutex_unlock(&_pool->lock);

    tpool_threadpool_wait_no_task(_pool);

    if (_pool != NULL && _pool->option != NULL && _pool->option_destroy != NULL) {
        _pool->option_destroy(_pool->option);
    }

    pthread_mutex_destroy(&_pool->lock);
    pthread_cond_destroy(&_pool->notify_task);
    pthread_cond_destroy(&_pool->notify_no_task);

    free(_pool);
    _pool = NULL;
}

void tpool_threadpool_add_thread(threadpool* _pool, void* _option, void(*_option_destroy)(void*))
{
    if (_pool == NULL) {
        return;
    }

    pthread_mutex_lock(&_pool->lock);
    pthread_t pthread;
    pthread_create(&pthread, NULL, tpool_thread_tasker, _pool);
    pthread_detach(pthread);
    threadpool_thread thread = { .pthread = pthread, .status = _pool->status_min_thread, .option = _option, .option_destroy = _option_destroy };
    linked_list_item* item = linked_list_item_create(&thread, sizeof(threadpool_thread));
    linked_list_append_item(_pool->threads, item);

    pthread_mutex_unlock(&_pool->lock);
}

void tpool_threadpool_remove_thread(threadpool* _pool)
{
    if (_pool == NULL) {
        return;
    }

    pthread_mutex_lock(&_pool->lock);
    size_t count_waiting = tpool_threadpool_get_count_status_thread(_pool, _pool->status_min_thread, _pool->status_min_thread);
    size_t count_tasking = tpool_threadpool_get_count_status_thread(_pool, _pool->status_min_thread + 1, _pool->status_max_thread);
    if (count_waiting > 0) {
        linked_list_item* item = linked_list_get_head(_pool->threads);
        size_t count = 0;
        while (item != NULL) {
            threadpool_thread* thread = linked_list_item_get_data(item);
            if (thread->status == _pool->status_min_thread) {
                pthread_cancel(thread->pthread);
                if (thread != NULL && thread->option != NULL && thread->option_destroy != NULL) {
                    thread->option_destroy(&thread->option);
                }
                linked_list_item* item = linked_list_get_ith(_pool->threads, count);
                linked_list_remove_item(_pool->threads, item, NULL, NULL);
                pthread_mutex_unlock(&_pool->lock);
                return;
            }
            item = linked_list_item_get_next(item);
            ++count;
        }
    }
    else if (count_tasking > 0) {
        linked_list_item* item = linked_list_get_head(_pool->threads);
        size_t count = 0;
        while (item != NULL) {
            threadpool_thread* thread = linked_list_item_get_data(item);
            if (thread->status > _pool->status_min_thread && thread->status < _pool->status_max_thread) {
                thread->status = _pool->status_max_thread;
                pthread_mutex_unlock(&_pool->lock);
                return;
            }
            item = linked_list_item_get_next(item);
            ++count;
        }
    }

    pthread_mutex_unlock(&_pool->lock);
}

// bool tpool_threadpool_add_thread_task(threadpool* _pool, action_task _action, void* _arg, void(*_arg_destroy)(void*), action_task _pre, action_task _post)
bool tpool_threadpool_add_thread_task(threadpool* _pool, thread_task* _task)
{
    if (_pool == NULL || _pool->threads == NULL || _task == NULL) {
        return false;
    }

    // thread_task task = { .pre = _pre, .action = _action, .post = _post, .arg = _arg, .arg_destroy = _arg_destroy };
    queue_item* item = queue_item_create(&_task, sizeof(thread_task*));
    // queue_item* item = tpool_queue_item_task_create(_action, _arg, _arg_destroy, _pre, _post);
    if (item == NULL) {
        return false;
    }

    pthread_mutex_lock(&_pool->lock);
    queue_put_item(_pool->tasks_waiting, item);

    pthread_cond_broadcast(&(_pool->notify_task));
    pthread_mutex_unlock(&_pool->lock);

    return true;
}

void tpool_threadpool_wait_no_task(threadpool *_pool)
{
    if (_pool == NULL) {
        return;
    }

    pthread_mutex_lock(&_pool->lock);
    while (true) {
        if (tpool_threadpool_get_count_status_thread(_pool, _pool->status_min_thread + 1, _pool->status_max_thread) != 0 || queue_get_count(_pool->tasks_waiting) != 0) {
            pthread_cond_wait(&(_pool->notify_no_task), &(_pool->lock));
        }
        else {
            break;
        }
    }
    pthread_mutex_unlock(&_pool->lock);
}


void* tpool_threadpool_get_thread_task_arg(threadpool* _pool, size_t _ith)
{
    threadpool_thread* thread = tpool_threadpool_get_thread_ith(_pool, _ith);
    if (thread != NULL) {
        thread_task* task = thread->task;
        if (task != NULL) {
            return task->arg;
        }
    }

    return NULL;
}

bool tpool_check_thread_timeout(thread_task* _task, struct timeval _time_current)
{
    if (_task == NULL) {
        return false;
    }

    struct timeval expire = tpool_task_get_expire(_task);
    if (expire.tv_sec != 0 && expire.tv_usec != 0) {
        if (expire.tv_sec < _time_current.tv_sec || (expire.tv_sec == _time_current.tv_sec && expire.tv_usec <= _time_current.tv_usec)) {
            return true;
        }
    }

    return false;
}

void tpool_restart_thread(threadpool* _pool, thread_task* _task)
{
    if (_pool == NULL || _task == NULL) {
        return;
    }

    linked_list* threads = tpool_threadpool_get_threads(_pool);
    linked_list_item* item = data_get_linked_list_item(_task);
    // if (linked_list_include_item(threads, _item) == false) {
    //     return;
    // }
    // threadpool_thread* thread = (threadpool_thread*)linked_list_item_get_data(_item);
    // pthread_t pthread = tpool_thread_get_pthread(thread);
    // thread_task* task = tpool_thread_get_task(thread);
    threadpool_thread* thread = tpool_task_get_thread(_task);
    pthread_t pthread = tpool_thread_get_pthread(thread);
    tpool_task_set_expire(_task, (struct timeval){0, 0});
    pthread_cancel(pthread);
    linked_list_remove_item(threads, item, NULL, NULL);
    tpool_threadpool_add_thread(_pool, NULL, NULL);
    tpool_threadpool_add_thread_task(_pool, _task);
}

int tpool_check_restart_threads_timeout(threadpool* _pool)
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
            tpool_restart_thread(_pool, task);
            ++restart;
        }
        item = item_next;
    }

    return restart;
}
