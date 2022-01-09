#ifndef ___SO_TEST_SERVER_WORKER_DEF_H___
#define ___SO_TEST_SERVER_WORKER_DEF_H___


#include "../../mmmAppExt.h"
#include "../../so/lib/mmmServerVariable/mmmServerVariableExt.h"
#include "../../so/app/subThread/subThreadExt.h"
#include "../../so/lib/rest/restExt.h"
#include "../../so/app/mmmPlugin/mmmPluginExt.h"

#ifdef __cplusplus
extern "C" {
#endif

    fsm_rcode server_send_req_plus_one(FSM_event* _evt);
    fsm_rcode worker_receive_req_plus_one(FSM_event* _evt);
    fsm_rcode worker_receive_plus_one_second(FSM_event* _evt);
    fsm_rcode server_receive_res_plus_one(FSM_event* _evt);
    fsm_rcode state_before_the_final_default(FSM_event* _evt);
    fsm_rcode state_final_default(FSM_event* _evt);

#ifdef __cplusplus
}
#endif

#endif // ___SO_TEST_SERVER_WORKER_DEF_H___
