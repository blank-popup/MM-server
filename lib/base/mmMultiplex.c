#include <string.h>
#include "mmMultiplex.h"


int epoll_add_event(int _epfd, int _fd, int _events)
{
    struct epoll_event ev = { 0 };
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.data.fd = _fd;
    ev.events = _events;
    return epoll_ctl(_epfd, EPOLL_CTL_ADD, _fd, &ev);
}

int epoll_add_ptr_event(int _epfd, int _fd, void* _ptr, int _events)
{
    struct epoll_event ev = { 0 };
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.data.ptr = _ptr;
    ev.events = _events;
    return epoll_ctl(_epfd, EPOLL_CTL_ADD, _fd, &ev);
}

int epoll_remove_event(int _epfd, int _fd)
{
    struct epoll_event ev = { 0 };
    memset(&ev, 0x00, sizeof(struct epoll_event));
    return epoll_ctl(_epfd, EPOLL_CTL_DEL, _fd, &ev);
}

int epoll_modify_event(int _epfd, int _fd, int _events)
{
    struct epoll_event ev = { 0 };
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.data.fd = _fd;
    ev.events = _events;
    return epoll_ctl(_epfd, EPOLL_CTL_MOD, _fd, &ev);
}

int epoll_modify_ptr_event(int _epfd, int _fd, void* _ptr, int _events)
{
    struct epoll_event ev = { 0 };
    memset(&ev, 0x00, sizeof(struct epoll_event));
    ev.data.ptr = _ptr;
    ev.events = _events;
    return epoll_ctl(_epfd, EPOLL_CTL_MOD, _fd, &ev);
}
