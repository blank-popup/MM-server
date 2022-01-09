#ifndef ___MMM_APP_SOCK_H___
#define ___MMM_APP_SOCK_H___

#include "mmmAppType.h"


#ifdef __cplusplus
extern "C" {
#endif

#define SOCK_HTTP_RESPONSE_500 "HTTP/1.1 500 Internal Server Error\r\n\r\n"

    typedef struct _epoll_ptr_info {
        ID_event maker;
        i32 fd;
    } epoll_ptr_info;


    rcode initialize_app_socket_server();
    rcode initialize_app_dsock_receive_fd_server();
    rcode initialize_app_dsock_send_fd_client();
    dsock_client_info* get_sendfd_dsock_client_info(i32 _number);
    dsock_client_info* get_sendfd_dsock_client_info_next_set_latest();
    rcode initialize_app_domain_socket_server();
    rcode initialize_app_domain_socket_client();

    rcode accept_send_fd();
    rcode receive_add_epoll_fd();

#ifdef __cplusplus
    }
#endif

#endif // ___MMM_APP_SOCK_H___
