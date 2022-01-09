#ifndef ___MM_KIT_URL_H___
#define ___MM_KIT_URL_H___

#include "kit/json/mmJson.h"


#ifdef __cplusplus
extern "C" {
#endif

    char* url_read_file(const char* _filepath);
    JSON_value* url_parse_content(char* _content);
    JSON_value* url_parse_file(const char* _filepath);

    JSON_object* url_get_function_process(JSON_object* _object, const char* _url);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_URL_H___
