#ifndef ___MM_KIT_EMAIL_H___
#define ___MM_KIT_EMAIL_H___

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


#ifdef  __cplusplus
extern "C" {
#endif

#define MAIL_SENDER "sender@gmail.com"
#define MAIL_USERNAME "user@gmail.com"
#define MAIL_PASSWORD "password"
#define MAIL_SMTP_SERVER "smtp.gmail.com"
#define MAIL_PORT_SSL (465)
#define MAIL_PORT_TLS (587)
#define MAIL_DOMAIN "localhost"
#define MAIL_X_MAILER "MM REST server"

#define MAIL_SIZE_SSL_READ (1024)
#define MAIL_SIZE_TLS_READ (1024)

    const char* mail_get_ip_address(const char* _target_domain);
    int mail_connect_to_server(const char* _server_address, uint16_t _port);
    char* mail_create_header(const char* _sender, const char* _receiver, const char* _subject, const char* _mime_type, const char* _charset);
    int mail_ssl_send(char buffer_receive[][MAIL_SIZE_SSL_READ], const char* _sender, char** _receiver, size_t _length_receiver, const char* _subject, char** _content, size_t _length_content);
    int mail_tls_send(char buffer_receive[][MAIL_SIZE_TLS_READ], const char* _sender, char** _receiver, size_t _length_receiver, const char* _subject, char** _content, size_t _length_content);
    int mail_communicate_common(char _buffer_receive[][MAIL_SIZE_SSL_READ], int _ith_start, SSL* _ssl, const char* _sender, char** _receiver, size_t _length_receiver, const char* _subject, char** _content, size_t _length_content);

#ifdef  __cplusplus
}
#endif

#endif // ___MM_KIT_EMAIL_H___
