#include "mmmAppExt.h"


rcode initialize_app_socket_server()
{
    u16 port = get_mmApp_self_socket_port();
    i32 backlog = get_mmApp_self_socket_backlog();
    if (port >= SOCKET_MIN_PORT && port <= SOCKET_MAX_PORT) {
        theApp.sds.sock.number = theApp.self.number;
        if ((theApp.sds.sock.fd = socket_server_initialize(port, backlog, SO_REUSEPORT, false)) < 0) {
            lerror(l_app, "Fail to initialize socket port[%u]", port);
            return mma(nak);
        }
        ldebug(l_app, "Initialized socket port[%u], fd[%d]", port, theApp.sds.sock);

        if (epoll_add_event(theApp.sds.ep, theApp.sds.sock.fd, EPOLLIN | EPOLLEXCLUSIVE) < 0) {
            lerror(l_app, "Fail to add event of socket port[%u], fd[%d]", theApp.sds.sock);
            return mma(nak);
        }
        linfo(l_app, "Added event of socket port[%u], fd[%d]", port, theApp.sds.sock.fd);
    }

    return mma(ack);
}

rcode initialize_app_dsock_receive_fd_server()
{
    const c8* path = get_mmApp_self_dsock_receive_fd_path();
    const i32 backlog = get_mmApp_self_dsock_receive_fd_backlog();

    if (STRING_IS_NOT_EMPTY(path)) {
        c8 filepath[MM_SIZE_PATH] = { 0, };
        const c8* directory_root = get_mm_directory_root();
        string_memset_print(filepath, sizeof(filepath), "%s/%s", directory_root, path);
        directory_make_recursively(filepath, sizeof(filepath), 0775);

        if ((theApp.sds.recvfd.fd = domain_socket_server_initialize(filepath, backlog, false)) < 0) {
            lerror(l_app, "Fail to initialize receiver msghdr path[%s]", path);
            return mma(nak);
        }
        ldebug(l_app, "Initialized receiver msghdr fd[%d], path[%s]", theApp.sds.recvfd.fd, path);

        if (epoll_add_event(theApp.sds.ep, theApp.sds.recvfd.fd, EPOLLIN | EPOLLEXCLUSIVE) < 0) {
            lerror(l_app, "Fail to add event of receiver msghdr to epoll fd[%d], path[%s]", theApp.sds.recvfd.fd, path);
            return mma(nak);
        }
        linfo(l_app, "Added event of receiver msghdr fd[%d], path[%s]", theApp.sds.recvfd.fd, path);
    }

    return mma(ack);
}

rcode initialize_app_dsock_send_fd_client()
{
    u16 port = get_mmApp_self_socket_port();
    if (port >= SOCKET_MIN_PORT && port <= SOCKET_MAX_PORT) {
        const c8* name_target = get_mmApp_self_socket_name_target();
        i32 type_target = process_name_get_type(name_target);
        if (type_target >= 0) {
            theApp.cds.sendfd.list = linked_list_create();
            theApp.cds.sendfd.size = get_mmApp_count_process(type_target);
            for (size_t ii = 0; ii < theApp.cds.sendfd.size; ++ii) {
                linked_list_item* item = linked_list_item_create(NULL, sizeof(dsock_client_info));
                linked_list_append_item(theApp.cds.sendfd.list, item);
                dsock_client_info* info = linked_list_item_get_data(item);
                info->number = process_get_number(type_target, ii);
                const c8* directory_root = get_mm_directory_root();
                const c8* path = get_mmApp_dsock_receive_fd_path(type_target, ii);
                string_memset_print(info->filepath_server, sizeof(info->filepath_server), "%s/%s", directory_root, path);
                ldebug(l_app, "Initialized domain socket client number: [%d], filepath server: [%s]", info->number, info->filepath_server);
            }
            linfo(l_app, "Initialized domain socket all client");
            theApp.cds.sendfd.item_latest = linked_list_get_tail(theApp.cds.sendfd.list);
        }
    }

    return mma(ack);
}

dsock_client_info* get_sendfd_dsock_client_info(i32 _number)
{
    for (size_t ii = 0; ii < theApp.cds.sendfd.size; ++ ii) {
        linked_list_item* item = linked_list_get_ith(theApp.cds.sendfd.list, ii);
        if (item == NULL) {
            return NULL;
        }
        dsock_client_info* info = linked_list_item_get_data(item);
        if (info != NULL && _number == info->number) {
            return info;
        }
    }

    return NULL;
}

dsock_client_info* get_sendfd_dsock_client_info_next_set_latest()
{
    linked_list_item* item = linked_list_item_get_next(theApp.cds.sendfd.item_latest);
    if (item == NULL) {
        item = linked_list_get_head(theApp.cds.sendfd.list);
    }
    theApp.cds.sendfd.item_latest = item;
    dsock_client_info* info = linked_list_item_get_data(item);

    return info;
}

rcode initialize_app_domain_socket_server()
{
    const c8* path = get_mmApp_self_domain_socket_path();
    const i32 backlog = get_mmApp_self_domain_socket_backlog();

    if (STRING_IS_NOT_EMPTY(path)) {
        c8 filepath[MM_SIZE_PATH] = { 0, };
        const c8* directory_root = get_mm_directory_root();
        string_memset_print(filepath, sizeof(filepath), "%s/%s", directory_root, path);
        directory_make_recursively(filepath, sizeof(filepath), 0775);

        theApp.sds.dsock.number = theApp.self.number;
        if ((theApp.sds.dsock.fd = domain_socket_server_initialize(filepath, backlog, false)) < 0) {
            lerror(l_app, "Fail to initialize domain socket path[%s]", path);
            return mma(nak);
        }
        linfo(l_app, "Initialized domain socket fd[%d], path[%s]", theApp.sds.dsock.fd, path);

        if (epoll_add_event(theApp.sds.ep, theApp.sds.dsock.fd, EPOLLIN | EPOLLEXCLUSIVE) < 0) {
            lerror(l_app, "Fail to add event of domain socket to epoll fd[%d], path[%s]", theApp.sds.dsock.fd, path);
            return mma(nak);
        }
        linfo(l_app, "Added event of domain socket fd[%d], path[%s]", theApp.sds.dsock.fd, path);
    }

    return mma(ack);
}

rcode initialize_app_domain_socket_client()
{
    u16 port = get_mmApp_self_socket_port();
    if (port == 0) {
        const c8* name_target = get_mmApp_self_socket_name_target();
        i32 type_target = process_name_get_type(name_target);
        if (type_target >= 0) {
            theApp.cds.dsock.list = linked_list_create();
            theApp.cds.dsock.size = get_mmApp_count_process(type_target);
            for (size_t ii = 0; ii < theApp.cds.dsock.size; ++ii) {
                linked_list_item* item = linked_list_item_create(NULL, sizeof(dsock_client_info));
                linked_list_append_item(theApp.cds.dsock.list, item);
                dsock_client_info* info = linked_list_item_get_data(item);
                info->number = process_get_number(type_target, ii);
                const c8* directory_root = get_mm_directory_root();
                const c8* path = get_mmApp_domain_socket_path(type_target, ii);
                string_memset_print(info->filepath_server, sizeof(info->filepath_server), "%s/%s", directory_root, path);
                ldebug(l_app, "Initialized domain socket client number: [%d], filepath server: [%s]", info->number, info->filepath_server);
            }
            linfo(l_app, "Initialized domain socket all client");
            theApp.cds.dsock.item_latest = linked_list_get_tail(theApp.cds.dsock.list);
        }
    }

    return mma(ack);
}



rcode accept_send_fd()
{
    i32 fd_accept;
    if ((fd_accept = socket_server_accept(theApp.sds.sock.fd)) < 0) {
        lerror(l_app, "%s", "Fail to accept");
        return mma(nak);
    }

    c8 ip_client[SOCKET_SIZE_IP] = { 0, };
    u16 port_client;
    socket_server_get_client_ip_port(ip_client, sizeof(ip_client), &port_client, fd_accept);

    i32 fd_dsock;
    if ((fd_dsock = domain_socket_client_initialize(false)) < 0) {
        lerror(l_app, "%s", "Fail to initialize domain socket client");
        return mma(nak);
    }

    size_t count_try = 0;
    dsock_client_info* info = NULL;
    while (true) {
        ++count_try;
        info = get_sendfd_dsock_client_info_next_set_latest();
        if (domain_socket_client_connect(fd_dsock, info->filepath_server) < 0) {
            lwarn(l_app,"Server domian socket is inactive number[%d], path[%s]", info->number, info->filepath_server);
            if (count_try >= theApp.cds.sendfd.size) {
                lerror(l_app, "%s", "All server domian socket is inactive");
                const c8* line_req = SOCK_HTTP_RESPONSE_500;
                send(fd_accept, line_req, strlen(line_req), 0);
                close(fd_accept);

                return mma(nak);
            }
            continue;
        }
        break;
    }
    ldebug(l_app, "Connected to receiver msghdr fd[%d], filepath server[%s]", fd_dsock, info->filepath_server);

    ID_event maker = { 0 };
    memset(&maker, 0xff, sizeof(ID_event));
    maker.number = theApp.self.number;
    generate_app_control_id(&maker.id);

    if (domain_socket_send_msghdr(fd_accept, fd_dsock, &maker, sizeof(ID_event)) < 0) {
        lerror(l_app, "%s", "Fail to send socket descriptor");
        return mma(nak);
    }

    linfo(l_app, LOGGING_HEAD_MARK " Get NEW Event[SOCK accept] " LOGGING_HEAD_MARK " sent fd[%d] maker: [%lu:%d] ip[%s:%u] to Process Number[%d]", fd_accept, maker.id, maker.number, ip_client, port_client, info->number);
    close(fd_accept);
    close(fd_dsock);

    return mma(ack);
}

rcode receive_add_epoll_fd()
{
    i32 fd_client;
    if ((fd_client = domain_socket_server_accept(theApp.sds.recvfd.fd)) < 0) {
        lerror(l_app, "Fail to accept receive fd[%d]", fd_client);
        return mma(nak);
    }
    ldebug(l_app, "Accepted receive fd[%d]", fd_client);

    ID_event maker = { 0 };
    i32 fd_received;
    if (domain_socket_receive_msghdr(&fd_received, fd_client, &maker, sizeof(ID_event)) < 0) {
        lerror(l_app, "%s", "Fail to receive socket descriptor");
        return mma(nak);
    }
    close(fd_client);
    linfo(l_app, LOGGING_HEAD_MARK " Get NEW Event[SOCK recv] " LOGGING_HEAD_MARK " received fd[%d] maker: [%lu:%d]", fd_received, maker.id, maker.number);

    epoll_ptr_info* epoll_ptr = memloc(sizeof(epoll_ptr_info));
    epoll_ptr->maker = maker;
    epoll_ptr->fd = fd_received;
    if (epoll_add_ptr_event(theApp.sds.ep, fd_received, epoll_ptr, EPOLLIN | EPOLLONESHOT) < 0) {
        lerror(l_app, "%s", "Fail to add epoll event");
        return mma(nak);
    }
    ldebug(l_app, "Added event of socket recv fd[%d]", fd_received);

    return mma(ack);
}
