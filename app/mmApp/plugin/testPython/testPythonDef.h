#ifndef ___SO_TEST_PYTHON_DEF_H___
#define ___SO_TEST_PYTHON_DEF_H___


#include "../../mmmAppExt.h"
#include "../../so/lib/mmmServerVariable/mmmServerVariableExt.h"
#include "../../so/app/subThread/subThreadExt.h"
#include "../../so/lib/rest/restExt.h"
#include "../../so/app/mmmPlugin/mmmPluginExt.h"

#ifdef __cplusplus
extern "C" {
#endif

    fsm_rcode server_execute_python(FSM_event* _evt);

#ifdef __cplusplus
}
#endif

#endif // ___SO_TEST_PYTHON_DEF_H___
