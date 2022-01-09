#ifndef ___SO_TEST_SERVER_DEF_H___
#define ___SO_TEST_SERVER_DEF_H___


#include "../../mmmAppExt.h"
#include "../../so/lib/mmmServerVariable/mmmServerVariableExt.h"
#include "../../so/app/subThread/subThreadExt.h"
#include "../../so/lib/rest/restExt.h"
#include "../../so/app/mmmPlugin/mmmPluginExt.h"

#ifdef __cplusplus
extern "C" {
#endif

    fsm_rcode help_start(FSM_event* _evt);
    fsm_rcode basic_start(FSM_event* _evt);
    fsm_rcode state_0_default(FSM_event* _evt);
    fsm_rcode state_1_default(FSM_event* _evt);
    fsm_rcode state_final_default(FSM_event* _evt);

#ifdef __cplusplus
}
#endif

#endif // ___SO_TEST_SERVER_DEF_H___
