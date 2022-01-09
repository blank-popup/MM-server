#ifndef ___MMM_PLUGIN_DEF_H___
#define ___MMM_PLUGIN_DEF_H___

#include "../../../mmmAppExt.h"
#include "../../lib/mmmServerVariable/mmmServerVariableExt.h"
#include "../../lib/rest/restExt.h"


#ifdef __cplusplus
extern "C" {
#endif

// #define MMP_ITH_PLUS_ONE (0)
// #define MMP_REQ_PLUS_ONE (MMP_REQ_BASE + MMP_ITH_PLUS_ONE)
// #define MMP_REQ_PLUS_ONE_STRING "req_plus_one"
// #define MMP_RES_PLUS_ONE (MMP_RES_BASE + MMP_ITH_PLUS_ONE)
// #define MMP_RES_PLUS_ONE_STRING "res_plus_one"

#define MMP_ITH_PLUS_ONE (0)
#define MMP_REQ_PLUS_ONE (MMP_REQ_BASE + MMP_ITH_PLUS_ONE)
#define MMP_RES_PLUS_ONE (MMP_RES_BASE + MMP_ITH_PLUS_ONE)

#define MMP_REQ_CMD_STRINGS (const c8*[]) {             \
    "req_plus_one",                                     \
    NULL                                                \
}

#define MMP_RES_CMD_STRINGS (const c8*[]) {             \
    "res_plus_one",                                     \
    NULL                                                \
}

    typedef struct _mmp_req_plus_one {
        i32 value;
    } mmp_req_plus_one;
    typedef struct _mmp_res_plus_one {
        i32 value;
    } mmp_res_plus_one;

    typedef union _mmp_rr_what {
        mmp_req_plus_one req_plus_one;
        mmp_res_plus_one res_plus_one;
    } mmp_rr_what;

    typedef struct _mmp_rr {
        i32 command;
        mm_rr common;
        i32 version;
        mmp_rr_what what;
    } mmp_rr;

    typedef struct _mmp_context {
        JSON_value* requests;
        JSON_value* responses;
    } mmp_context;

    // rcode process_message_queue_mmp(c8* _msg, FSM* _fsm);

    rcode plugin_set_req(FSM_event* _evt);
    rcode plugin_process(FSM_event* _evt);
    rcode plugin_send_outcome(FSM_event* _evt);
    rcode plugin_variable_free(FSM_event* _evt);

    const c8* get_mmp_rr_cmd_strings(i32 _command);
    mmp_rr mmp_rr_make(i32 _command, mm_rr _rrc, i32 _version);
    mmp_rr* mmp_rr_create(i32 _command, mm_rr _rrc, i32 _version);
    void mmp_rr_destroy(mmp_rr* _rr);
    // mmp_context* mmp_context_create();
    // void mmp_context_destroy();

#ifdef __cplusplus
}
#endif

#endif // ___MMM_PLUGIN_DEF_H___
