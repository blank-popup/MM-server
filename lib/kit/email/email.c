#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../base64/base64.h"
#include "email.h"


const char* mail_get_ip_address(const char* _target_domain)
{
    const char* target_ip;
    struct hostent* raw_list = gethostbyname(_target_domain);
    for (int ii = 0; raw_list->h_addr_list[ii] != 0; ++ii) {
        struct in_addr* host_address = (struct in_addr*)raw_list->h_addr_list[ii];
        target_ip = inet_ntoa(*host_address);
    }
    return target_ip;
}

int mail_connect_to_server(const char* _server_address, uint16_t _port)
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (fd == -1) {
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    if (inet_pton(AF_INET, mail_get_ip_address(_server_address), &addr.sin_addr) == 1) {
        connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    }
    return fd;
}

char* mail_create_header(const char* _sender, const char* _receiver, const char* _subject, const char* _mime_type, const char* _charset)
{
    if (_sender == NULL || _receiver == NULL) {
        return NULL;
    }

    if (_subject == NULL || strcmp(_subject, "") == 0) {
        _subject = "No title";
    }
    if (_mime_type == NULL || strcmp(_mime_type, "") == 0) {
        _mime_type = "text/plain";
    }
    if (_charset == NULL || strcmp(_charset, "") == 0) {
        _charset = "US-ASCII";
    }

    time_t now;
    time(&now);
    const char* x_mailer = MAIL_X_MAILER;
    char* mail_header = NULL;
    size_t size_date = 33;
    char buffer_date[size_date];
    memset(buffer_date, 0x00, sizeof(buffer_date));
    char date_x_mailer[6 + strlen(buffer_date) + 12 + strlen(x_mailer) + 3];
    memset(date_x_mailer, 0x00, sizeof(date_x_mailer));
    char sender[6 + strlen(_sender) + 3];
    memset(sender, 0x00, sizeof(sender));
    char receiver[4 + strlen(_receiver) + 3];
    memset(receiver, 0x00, sizeof(receiver));
    char subject[9 + strlen(_subject) + 3];
    memset(subject, 0x00, sizeof(subject));
    char mime_data[33 + strlen(_mime_type) + 10 + strlen(_charset) + 5];
    memset(mime_data, 0x00, sizeof(mime_data));

    strftime(buffer_date, size_date, "%a, %d %b %Y %H:%M:%S", localtime(&now));

    sprintf(date_x_mailer, "DATE: %s\r\nX-Mailer: %s\r\n", buffer_date, x_mailer);
    sprintf(sender, "FROM: %s\r\n", _sender);
    sprintf(receiver, "To: %s\r\n", _receiver);
    sprintf(subject, "Subject: %s\r\n", _subject);
    sprintf(mime_data, "MIME-Version: 1.0\r\nContent-type: %s; charset=%s\r\n\r\n", _mime_type, _charset);

    size_t length_mail_header = strlen(date_x_mailer) + strlen(sender) + strlen(receiver) + strlen(subject) + strlen(mime_data) + 1;

    mail_header = malloc(length_mail_header * sizeof(char));
    sprintf(mail_header + strlen(mail_header), "%s", date_x_mailer);
    sprintf(mail_header + strlen(mail_header), "%s", sender);
    sprintf(mail_header + strlen(mail_header), "%s", receiver);
    sprintf(mail_header + strlen(mail_header), "%s", subject);
    sprintf(mail_header + strlen(mail_header), "%s", mime_data);

    return mail_header;
}

int mail_ssl_send(char buffer_receive[][MAIL_SIZE_SSL_READ], const char* _sender, char** _receiver, size_t _length_receiver, const char* _subject, char** _content, size_t _length_content)
{
    if (_sender == NULL || _receiver == NULL) {
        return 1;
    }

    if (SSL_library_init() < 0) {
        return 2;
    }
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    const SSL_METHOD* method = SSLv23_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        return 3;
    }

    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        SSL_CTX_free(ctx);
        return 4;
    }

    int fd = mail_connect_to_server(MAIL_SMTP_SERVER, MAIL_PORT_SSL);
    if (fd == -1) {
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 5;
    }

    SSL_set_fd(ssl, fd);
    if (SSL_connect(ssl) != 1) {
        close(fd);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        return 6;
    }

    int ith = 0;

    memset(buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(ssl, (void*)buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    char helo[5 + strlen(MAIL_DOMAIN) + 3];
    memset(helo, 0x00, sizeof(helo));
    sprintf(helo, "%s", "HELO ");
    sprintf(helo + strlen(helo), "%s", MAIL_DOMAIN);
    sprintf(helo + strlen(helo), "%s", "\r\n");
    SSL_write(ssl, helo, strlen(helo));
    memset(buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(ssl, (void*)buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    mail_communicate_common(buffer_receive, ith, ssl, _sender, _receiver, _length_receiver, _subject, _content, _length_content);

    close(fd);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}

int mail_tls_send(char buffer_receive[][MAIL_SIZE_TLS_READ], const char* _sender, char** _receiver, size_t _length_receiver, const char* _subject, char** _content, size_t _length_content)
{
    if (_sender == NULL || _receiver == NULL) {
        return 1;
    }

    if (SSL_library_init() < 0) {
        return 2;
    }
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

    const SSL_METHOD* method = SSLv23_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        return 3;
    }

    int fd = mail_connect_to_server(MAIL_SMTP_SERVER, MAIL_PORT_TLS);
    if (fd == -1) {
        SSL_CTX_free(ctx);
        return 4;
    }

    int ith = 0;

    memset(buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    recv(fd, buffer_receive[ith++], MAIL_SIZE_SSL_READ, 0);

    char ehlo0[5 + strlen(MAIL_DOMAIN) + 3];
    memset(ehlo0, 0x00, sizeof(ehlo0));
    sprintf(ehlo0, "%s", "EHLO ");
    sprintf(ehlo0 + strlen(ehlo0), "%s", MAIL_DOMAIN);
    sprintf(ehlo0 + strlen(ehlo0), "%s", "\r\n");
    send(fd, ehlo0, strlen(ehlo0), 0);
    memset(buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    recv(fd, buffer_receive[ith++], MAIL_SIZE_SSL_READ, 0);

    const char* starttls = "STARTTLS\r\n";
    send(fd, starttls, strlen(starttls), 0);
    memset(buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    recv(fd, buffer_receive[ith++], MAIL_SIZE_SSL_READ, 0);

    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);
    SSL* ssl = SSL_new(ctx);
    if (ssl == NULL) {
        close(fd);
        SSL_CTX_free(ctx);
        return 5;
    }

    SSL_set_fd(ssl, fd);
    if (SSL_connect(ssl) != 1) {
        SSL_free(ssl);
        close(fd);
        SSL_CTX_free(ctx);
        return 6;
    }

    char ehlo1[5 + strlen(MAIL_DOMAIN) + 3];
    memset(ehlo1, 0x00, sizeof(ehlo1));
    sprintf(ehlo1, "%s", "EHLO ");
    sprintf(ehlo1 + strlen(ehlo1), "%s", MAIL_DOMAIN);
    sprintf(ehlo1 + strlen(ehlo1), "%s", "\r\n");
    SSL_write(ssl, ehlo1, strlen(ehlo1));
    memset(buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    int rc = SSL_read(ssl, (void*)buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    mail_communicate_common(buffer_receive, ith, ssl, _sender, _receiver, _length_receiver, _subject, _content, _length_content);

    SSL_free(ssl);
    close(fd);
    SSL_CTX_free(ctx);

    return 0;
}

int mail_communicate_common(char _buffer_receive[][MAIL_SIZE_SSL_READ], int _ith_start, SSL* _ssl, const char* _sender, char** _receiver, size_t _length_receiver, const char* _subject, char** _content, size_t _length_content)
{
    int ith = _ith_start;

    const char* auth_login = "AUTH LOGIN\r\n";
    SSL_write(_ssl, auth_login, strlen(auth_login));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    char username[strlen(MAIL_USERNAME) + 3];
    memset(username, 0x00, sizeof(username));
    base64_encode(username, MAIL_USERNAME, strlen(MAIL_USERNAME));
    sprintf(username + strlen(username), "%s", "\r\n");
    SSL_write(_ssl, username, strlen(username));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    char password[strlen(MAIL_PASSWORD) + 3];
    memset(password, 0x00, sizeof(password));
    base64_encode(password, MAIL_PASSWORD, strlen(MAIL_PASSWORD));
    sprintf(password + strlen(password), "%s", "\r\n");
    SSL_write(_ssl, password, strlen(password));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    char mail_from[12 + strlen(_sender) + 4];
    memset(mail_from, 0x00, sizeof(mail_from));
    sprintf(mail_from, "%s", "MAIL FROM: ");
    sprintf(mail_from + strlen(mail_from), "<%s>", _sender);
    sprintf(mail_from + strlen(mail_from), "%s", "\r\n");
    SSL_write(_ssl, mail_from, strlen(mail_from));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    for (size_t ii = 0; ii < _length_receiver; ++ii) {
        char rctp_to[9 + strlen(_receiver[ii]) + 4];
        memset(rctp_to, 0x00, sizeof(rctp_to));
        sprintf(rctp_to, "%s", "RCPT TO: ");
        sprintf(rctp_to + strlen(rctp_to), "<%s>", _receiver[ii]);
        sprintf(rctp_to + strlen(rctp_to), "%s", "\r\n");
        SSL_write(_ssl, rctp_to, strlen(rctp_to));
        memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
        SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);
    }

    const char* data = "DATA\r\n";
    SSL_write(_ssl, data, strlen(data));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    size_t length_receivers = 0;
    for (size_t ii = 0; ii < _length_receiver; ++ii) {
        length_receivers += strlen(_receiver[ii]);
        if (ii < _length_receiver - 1) {
            length_receivers += strlen(", ");
        }
    }
    char* receivers = malloc((length_receivers + 1) * sizeof(char));
    memset(receivers, 0x00, (length_receivers + 1) * sizeof(char));
    for (size_t ii = 0; ii < _length_receiver; ++ii) {
        sprintf(receivers + strlen(receivers), "%s", _receiver[ii]);
        if (ii < _length_receiver - 1) {
            sprintf(receivers + strlen(receivers), "%s", ", ");
        }
    }
    char* header = mail_create_header(_sender, receivers, _subject, "text/plain", "US-ASCII");
    SSL_write(_ssl, header, strlen(header));
    free(header);
    free(receivers);
    for (size_t ii = 0; ii < _length_content; ++ii) {
        SSL_write(_ssl, _content[ii], strlen(_content[ii]));
    }
    const char* dot = "\r\n.\r\n";
    SSL_write(_ssl, dot, strlen(dot));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    const char* quit = "QUIT\r\n";
    SSL_write(_ssl, quit, strlen(quit));
    memset(_buffer_receive[ith], 0x00, MAIL_SIZE_SSL_READ);
    SSL_read(_ssl, (void*)_buffer_receive[ith++], MAIL_SIZE_SSL_READ);

    return 0;
}
