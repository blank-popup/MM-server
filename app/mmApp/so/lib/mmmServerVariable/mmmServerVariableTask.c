#include "mmmServerVariableExt.h"


rcode mm_rest_create(const c8* _filepath_immanence)
{
    if (___mm_rest != NULL || _filepath_immanence == NULL) {
        return mma(nak);
    }
    JSON_value* value_immanence = json_parse_file(_filepath_immanence);
    JSON_object* object_immanence = json_value_get_object(value_immanence);
    JSON_value* value_rest = json_object_get_value(object_immanence, NAME_REST);
    if (value_rest == NULL) {
        return mma(nak);
    }
    ___mm_rest = json_value_duplicate(value_rest);

    json_value_free(value_immanence);

    return mma(ack);
}

JSON_value* mm_rest_get()
{
    return ___mm_rest;
}

void mm_rest_destroy()
{
    if (___mm_rest == NULL) {
        return;
    }
    json_value_free(___mm_rest);
}