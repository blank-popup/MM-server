#ifndef ___MMM_APP_EXT_H___
#define ___MMM_APP_EXT_H___

#include "mmmAppDef.h"


#ifdef __cplusplus
extern "C" {
#endif

    extern config_application theApp;
    extern action_return ___return_app;
    extern action_return ___return_so;

    extern JSON_object* l_app;
    extern JSON_object* l_so;

#ifdef __cplusplus
}
#endif

#endif // ___MMM_APP_EXT_H___
