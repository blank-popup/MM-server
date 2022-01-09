#ifndef ___SO_TEST_ID_DEF_H___
#define ___SO_TEST_ID_DEF_H___


#include "../../mmmAppExt.h"
#include "../../so/lib/mmmServerVariable/mmmServerVariableExt.h"
#include "../../so/app/subThread/subThreadExt.h"
#include "../../so/lib/rest/restExt.h"
#include "../../so/app/mmmPlugin/mmmPluginExt.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _item_type {
        i32 id;
        c8 name[18];
        i32 use;
    } item_type;

    typedef struct _item_cube {
        i32 id;
        c8 name[18];
        i32 length;
        i32 width;
        i32 height;
    } item_cube;

    item_type* search_1234(FSM_event* _evt);
    item_cube* search_5678(FSM_event* _evt);

    fsm_rcode get_volume(FSM_event* _evt);

#ifdef __cplusplus
}
#endif

#endif // ___SO_TEST_ID_DEF_H___
