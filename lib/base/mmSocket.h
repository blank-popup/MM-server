#ifndef ___MM_BASE_SOCKET_H___
#define ___MM_BASE_SOCKET_H___

#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

#define SOCKET_MIN_PORT (1024)
#define SOCKET_MAX_PORT (49151)
#define SOCKET_SIZE_IP (16)
#define SOCKET_SIZE_PORT (6)
#define SOCKET_SIZE_RECV_BUFFER (1024)
#define IOVEC_DEFAULT_IO_SIZE (64)

    int socket_make_non_blocking(int _fd);
    int domain_socket_server_initialize(const char* _path, int _backlog, bool _nonblocking);
    int domain_socket_client_initialize(bool _nonblocking);
    int domain_socket_client_connect(int _fd, const char* _path);
    int domain_socket_server_accept(int _fd);
    int socket_server_initialize(unsigned short _port, int _backlog, int _optname, bool _nonblocking);
    int socket_client_initialize(bool _nonblocking);
    int socket_client_connect(int _fd, const char* _ip, unsigned short _port);
    int socket_server_accept(int _fd);
    int socket_server_get_client_ip_port(char* _ip_client, size_t _size_ip_client, unsigned short* _port_client, int _fd_client);

    int domain_socket_send_fd(int _fd_sent, int _fd);
    int domain_socket_receive_fd(int* _fd_received, int _fd);

    int domain_socket_send_msghdr(int _fd_sent, int _fd, void* _buffer, size_t _length_buffer);
    int domain_socket_receive_msghdr(int* _fd_received, int _fd, void* _buffer, size_t _length_buffer);

    int socket_nonblocking_recv_all(char** _data, size_t _size, int _fd);
    int socket_nonblocking_recv_all_timeout(char** _data, size_t _size, int _fd, double _timeout, useconds_t _diapause);

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_SOCKET_H___
