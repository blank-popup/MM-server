#include "mmmPluginExt.h"


const c8* get_mmp_rr_cmd_strings(i32 _command)
{
    if (_command >= MMP_REQ_BASE && _command < MMP_RES_BASE) {
        return ___mm_mmp_req_cmd_strings[_command - MMP_REQ_BASE];
    }
    else if (_command >= MMP_RES_BASE && _command < MMP_REQ_BASE + MM_SIZE_RR_PLUGIN) {
        return ___mm_mmp_res_cmd_strings[_command - MMP_RES_BASE];
    }
    else {
        return NULL;
    }
}

mmp_rr mmp_rr_make(i32 _command, mm_rr _rrc, i32 _version)
{
    mmp_rr rr;
    rr.command = _command;
    rr.common = _rrc;
    rr.version = _version;

    return rr;
}

mmp_rr* mmp_rr_create(i32 _command, mm_rr _rrc, i32 _version)
{
    mmp_rr* rr = memloc(sizeof(mma_rr));
    if (rr == NULL) {
        return NULL;
    }
    rr->command = _command;
    rr->common = _rrc;
    rr->version = _version;

    return rr;
}

void mmp_rr_destroy(mmp_rr* _rr)
{
    if (_rr != NULL) {
        memfree(_rr);
    }
}

// mmp_context* mmp_context_create()
// {
//     ___mmp_context = memloc(sizeof(mmp_context));
//     ___mmp_context->requests = json_value_create_array();
//     ___mmp_context->responses = json_value_create_array();

//     JSON_array* requests = json_value_get_array(___mmp_context->requests);
//     JSON_array* responses = json_value_get_array(___mmp_context->responses);

//     json_array_append_string(requests, MMP_REQ_PLUS_ONE_STRING);
//     json_array_append_string(responses, MMP_RES_PLUS_ONE_STRING);

//     return ___mmp_context;
// }

// void mmp_context_destroy()
// {
//     if (___mmp_context != NULL) {
//         if (___mmp_context->responses != NULL) {
//             json_value_free(___mmp_context->responses);
//         }
//         if (___mmp_context->requests != NULL) {
//             json_value_free(___mmp_context->requests);
//         }
//         memfree(___mmp_context);
//     }
// }
