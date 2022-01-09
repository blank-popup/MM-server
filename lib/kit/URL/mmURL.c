#include <string.h>
#include "mmURL.h"


char* url_read_file(const char* _filepath)
{
    return json_read_file(_filepath);
}

JSON_value* url_parse_content(char* _content)
{
    return json_parse_content(_content);
}

JSON_value* url_parse_file(const char* _filepath)
{
    return json_parse_file(_filepath);
}


JSON_object* url_get_function_process(JSON_object* _object, const char* _url)
{
    if (_object == NULL || _url == NULL || _url[0] != '/' || strcmp(_url, "/") == 0) {
        return NULL;
    }

    char* current = (char*)_url + 1;
    size_t size = 0;
    while (current != NULL && current[size] != '/') {
        ++current;
        ++size;
    }

    JSON_object* object = NULL;
    size_t length_object = json_object_get_length(_object);
    for (size_t ii = 0; ii < length_object; ++ii) {
        const char* name = json_object_get_name(_object, ii);
        if (strncmp(_url + 1, name, strlen(name)) == 0) {
            object = json_object_get_object(_object, name);
            break;
        }
    }

    if (current == NULL) {
        return object;
    }
    else {
        return url_get_function_process(object, current);
    }
}
