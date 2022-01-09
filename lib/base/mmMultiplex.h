#ifndef ___MM_BASE_MULTIPLEX_H___
#define ___MM_BASE_MULTIPLEX_H___

#include <sys/epoll.h>


#ifdef __cplusplus
extern "C" {
#endif

    int epoll_add_event(int _epfd, int _fd, int _events);
    int epoll_add_ptr_event(int _epfd, int _fd, void* _ptr, int _events);
    int epoll_remove_event(int _epfd, int _fd);
    int epoll_modify_event(int _epfd, int _fd, int _events);
    int epoll_modify_ptr_event(int _epfd, int _fd, void* _ptr, int _events);

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_MULTIPLEX_H___
