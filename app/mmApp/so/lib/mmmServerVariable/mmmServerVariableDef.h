#ifndef ___MMM_SERVER_VARIABLE_DEF_H___
#define ___MMM_SERVER_VARIABLE_DEF_H___

#include "../../../mmmAppExt.h"


#ifdef __cplusplus
extern "C" {
#endif

    rcode mm_rest_create(const c8* _filepath_immanence);
    JSON_value* mm_rest_get();
    void mm_rest_destroy();

#ifdef __cplusplus
}
#endif

#endif // ___MMM_SERVER_VARIABLE_DEF_H___
