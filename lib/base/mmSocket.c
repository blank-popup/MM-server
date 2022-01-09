#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include "mmSocket.h"
#include "base/mmMemory.h"
#include "base/mmString.h"
#include "../mmLogSimple.h"


static bool fd_is_valid(int _fd)
{
    return fcntl(_fd, F_GETFD) != -1;
}

int socket_make_non_blocking(int _fd)
{
    int flags;
    if ((flags = fcntl(_fd, F_GETFL, 0)) < 0) {
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(_fd, F_SETFL, flags) < 0) {
        return -1;
    }

    return 0;
}

int domain_socket_server_initialize(const char* _path, int _backlog, bool _nonblocking)
{
    if (_path == NULL) {
        return -1;
    }

    struct sockaddr_un sun;

    int fd = -1;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if (_nonblocking == true) {
        if (socket_make_non_blocking(fd) < 0) {
            return -1;
        }
    }

    memset(&sun, 0x00, sizeof(sun));
    sun.sun_family = AF_UNIX;
    strncpy(sun.sun_path, _path, sizeof(sun.sun_path) - 1);

    unlink(_path);
    if (bind(fd, (struct sockaddr*)&sun, sizeof(sun)) < 0) {
        return -1;
    }
    if (listen(fd, _backlog) < 0) {
        return -1;
    }

    return fd;
}

int domain_socket_client_initialize(bool _nonblocking)
{
    int fd = -1;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if (_nonblocking == true) {
        if (socket_make_non_blocking(fd) < 0) {
            return -1;
        }
    }

    return fd;
}

int domain_socket_client_connect(int _fd, const char* _path)
{
    if (_path == NULL) {
        return -1;
    }

    struct sockaddr_un sun;
    memset(&sun, 0x00, sizeof(struct sockaddr_un));
    sun.sun_family = AF_UNIX;
    memcpy(&sun.sun_path, _path, strlen(_path));

    if (connect(_fd, (struct sockaddr*)&sun, sizeof(struct sockaddr_un)) < 0) {
        return -1;
    }

    return 0;
}

int domain_socket_server_accept(int _fd)
{
    struct sockaddr_un sun;
    socklen_t socklen = sizeof(struct sockaddr_un);

    int fd_client = -1;
    if ((fd_client = accept(_fd, (struct sockaddr*)&sun, &socklen)) < 0) {
        return -1;
    }

    return fd_client;
}

int socket_server_initialize(unsigned short _port, int _backlog, int _optname, bool _nonblocking)
{
    socklen_t socklen = sizeof(struct sockaddr);
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(_port);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&(sin.sin_zero), 0x00, sizeof(sin.sin_zero));

    int fd = -1;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if (_nonblocking == true) {
        if (socket_make_non_blocking(fd) < 0) {
            return -1;
        }
    }
    int optval = 1;
    if (setsockopt(fd, SOL_SOCKET, _optname, &optval, sizeof(optval)) < 0) {
        return -1;
    }
    if (bind(fd, (struct sockaddr*)&sin, socklen) < 0) {
        return -1;
    }
    if (listen(fd, _backlog) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}

int socket_client_initialize(bool _nonblocking)
{
    int fd = -1;
    if ((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if (_nonblocking == true) {
        if (socket_make_non_blocking(fd) < 0) {
            return -1;
        }
    }

    return fd;
}

int socket_client_connect(int _fd, const char* _ip, unsigned short _port)
{
    if (_ip == NULL) {
        return -1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0x00, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(_ip);
    sin.sin_port = htons(_port);

    if (connect(_fd, (struct sockaddr*)&sin, sizeof(struct sockaddr_in)) < 0) {
        return -1;
    }

    return 0;
}

int socket_server_accept(int _fd)
{
    struct sockaddr_in sin;
    socklen_t socklen = sizeof(struct sockaddr_in);

    int fd = -1;
    if ((fd = accept(_fd, (struct sockaddr*)&sin, &socklen)) < 0) {
        return -1;
    }

    return fd;
}

int socket_server_get_client_ip_port(char* _ip_client, size_t _size_ip_client, unsigned short* _port_client, int _fd_client)
{
    if (_ip_client == NULL && _port_client == NULL) {
        return -1;
    }

    struct sockaddr_in addr;
    socklen_t size_addr = sizeof(struct sockaddr_in);
    if (getpeername(_fd_client, (struct sockaddr*)&addr, &size_addr) < 0) {
        return -1;
    }
    if (_ip_client != NULL) {
        memset(_ip_client, 0x00, _size_ip_client);
        strncpy(_ip_client, inet_ntoa(addr.sin_addr), _size_ip_client - 1);
    }
    if (_port_client != NULL) {
        *_port_client = ntohs(addr.sin_port);
    }

    return 0;
}


int domain_socket_send_fd(int _fd_sent, int _fd)
{
    struct msghdr msg = {0};
    struct cmsghdr* cmsg;
    char buf[CMSG_SPACE(sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));

    memcpy((int*)CMSG_DATA(cmsg), &_fd_sent, sizeof(int));

    if (sendmsg(_fd, &msg, 0) < 0) {
        return -1;
    }

    return 0;
}

int domain_socket_receive_fd(int* _fd_received, int _fd)
{
    if (_fd_received == NULL) {
        return -1;
    }

    struct msghdr msg = { 0 };
    struct cmsghdr* cmsg;
    char buf[CMSG_SPACE(sizeof(int))], dup[256];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = &dup, .iov_len = sizeof(dup) };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    if (recvmsg (_fd, &msg, 0) < 0)
        return -1;

    cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg == NULL) {
        return -1;
    }

    memcpy(_fd_received, (int*)CMSG_DATA(cmsg), sizeof(int));

    return 0;
}


int domain_socket_send_msghdr(int _fd_sent, int _fd, void* _buffer, size_t _length_buffer)
{
    struct msghdr msg = { 0 };
    struct iovec io = { 0 };
    if (_buffer == NULL) {
        char empty[IOVEC_DEFAULT_IO_SIZE] = { 0 };
        io.iov_base = empty;
        io.iov_len = sizeof(empty);
    }
    else {
        io.iov_base = _buffer;
        io.iov_len = _length_buffer;
    }

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    if (fd_is_valid(_fd_sent) == true) {
        char buffer_cmsg[CMSG_SPACE(sizeof(int))];
        memset(buffer_cmsg, 0x00, sizeof(buffer_cmsg));

        msg.msg_control = buffer_cmsg;
        msg.msg_controllen = sizeof(buffer_cmsg);

        struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int));

        memcpy((int*)CMSG_DATA(cmsg), &_fd_sent, sizeof(int));
    }

    int rrr = sendmsg(_fd, &msg, 0);
    if (rrr < 0) {
        return -1;
    }

    return 0;
}

int domain_socket_receive_msghdr(int* _fd_received, int _fd, void* _buffer, size_t _length_buffer)
{
    if (_fd_received == NULL) {
        return -1;
    }

    struct msghdr msg = { 0 };
    struct iovec io = { 0 };
    if (_buffer == NULL) {
        char empty[IOVEC_DEFAULT_IO_SIZE] = { 0 };
        io.iov_base = empty;
        io.iov_len = sizeof(empty);
    }
    else {
        io.iov_base = _buffer;
        io.iov_len = _length_buffer;
    }

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char buffer_cmsg[CMSG_SPACE(sizeof(int))];
    memset(buffer_cmsg, 0x00, sizeof(buffer_cmsg));

    msg.msg_control = buffer_cmsg;
    msg.msg_controllen = sizeof(buffer_cmsg);

    if (recvmsg (_fd, &msg, 0) < 0)
        return -1;

    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg != NULL) {
        memcpy(_fd_received, (int*)CMSG_DATA(cmsg), sizeof(int));
    }

    return 0;
}


int socket_nonblocking_recv_all(char** _data, size_t _size, int _fd)
{
    *_data = memloc(_size * sizeof(char));
    while (true) {
        char buffer[SOCKET_SIZE_RECV_BUFFER] = { 0, };
        int nread = recv(_fd, buffer, SOCKET_SIZE_RECV_BUFFER - 1, 0);
        if (nread == 0) {
            close(_fd);
            return -1;
        }
        else if (nread < 0) {
            if (errno == EAGAIN) {
                break;
            }
            else {
                continue;
            }
        }

        if (_size < strlen(*_data) + strlen(buffer) + 1) {
            memory_double_capacity((void**)_data, _size);
        }

        memcpy(*_data + strlen(*_data), buffer, strlen(buffer));
        (*_data)[strlen(*_data)] = '\0';
    }

    return 0;
}

int socket_nonblocking_recv_all_timeout(char** _data, size_t _size, int _fd, double _timeout, useconds_t _diapause)
{
    int size_total_recv = 0;
    struct timeval timeval_start , timeval_now;
    gettimeofday(&timeval_start , NULL);

    *_data = memloc(_size * sizeof(char));
    while (true) {
        gettimeofday(&timeval_now , NULL);
        double timediff = (timeval_now.tv_sec - timeval_start.tv_sec) + 1e-6 * (timeval_now.tv_usec - timeval_start.tv_usec);

        if(size_total_recv > 0 && timediff > _timeout ) {
            break;
        }
        else if(timediff > _timeout * 2) {
            break;
        }

        char buffer[SOCKET_SIZE_RECV_BUFFER] = { 0, };
        int nread = recv(_fd, buffer, SOCKET_SIZE_RECV_BUFFER - 1, 0);
        if (nread == 0) {
            close(_fd);
            return -1;
        }
        else if (nread < 0) {
            usleep(_diapause);
        }
        else {
            if (_size < strlen(*_data) + strlen(buffer) + 1) {
                memory_double_capacity((void**)_data, _size);
            }

            memcpy(*_data + strlen(*_data), buffer, strlen(buffer));
            (*_data)[strlen(*_data)] = '\0';

            size_total_recv += nread;
            gettimeofday(&timeval_start , NULL);
        }
    }

    return 0;
}
