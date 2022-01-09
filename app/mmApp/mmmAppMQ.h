#ifndef ___MMM_APP_MQ_H___
#define ___MMM_APP_MQ_H___

#include "mmmAppType.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MM_RR_BASE (0)
#define MM_SIZE_RR_APP (1 << 12)
#define MM_SIZE_RR_PLUGIN (1 << 16)
#define MMA_REQ_BASE (MM_RR_BASE)
#define MMA_RES_BASE (MM_RR_BASE + MM_SIZE_RR_APP)
#define MMP_REQ_BASE (MM_RR_BASE + MM_SIZE_RR_APP + MM_SIZE_RR_APP)
#define MMP_RES_BASE (MM_RR_BASE + MM_SIZE_RR_APP + MM_SIZE_RR_APP + MM_SIZE_RR_PLUGIN)
#define MM_RR_ERROR (-1)

#define MM_EXPAND(s) s
#define MM_CONCAT(e, t) e##_##t

#define MMA_ITH_NUMBER_PID (0)
#define MMA_REQ_NUMBER_PID (MMA_REQ_BASE + MMA_ITH_NUMBER_PID)
#define MMA_RES_NUMBER_PID (MMA_RES_BASE + MMA_ITH_NUMBER_PID)

#define MMA_ITH_LOGGING (1)
#define MMA_REQ_LOGGING (MMA_REQ_BASE + MMA_ITH_LOGGING)
#define MMA_RES_LOGGING (MMA_RES_BASE + MMA_ITH_LOGGING)

#define MMA_ITH_STATUS_PROCESS (2)
#define MMA_REQ_STATUS_PROCESS (MMA_REQ_BASE + MMA_ITH_STATUS_PROCESS)
#define MMA_RES_STATUS_PROCESS (MMA_RES_BASE + MMA_ITH_STATUS_PROCESS)

#define MMA_REQ_CMD_STRINGS (const c8*[]) {             \
    "req_number_pid",                                   \
    "req_logging",                                      \
    "req_status_process",                               \
    NULL                                                \
}

#define MMA_RES_CMD_STRINGS (const c8*[]) {             \
    "res_number_pid",                                   \
    "res_logging",                                      \
    "res_status_process",                               \
    NULL                                                \
}

    typedef struct _mm_rr {
        ID_event maker;
        ID_event sender;
        i32 receiver;
        bool need_response;
        struct timeval when;
        ID_event verifier;
    } mm_rr;

    typedef struct _mma_req_number_pid {
    } mma_req_number_pid;
    typedef struct _mma_res_number_pid {
        pid_t pid;
    } mma_res_number_pid;

#define LOGGING_EVENT_PROCESS_TERMINATED (0)
#define LOGGING_EVENT_PROCESS_REVIVED (1)

#define LOGGING_EVENT_STRINGS (const c8*[]) {           \
    "process terminated",                               \
    "process revived",                                  \
    NULL                                                \
}

    typedef struct _mma_req_logging {
        i32 event;
        logger_level level;
        i32 number;
        pid_t pid;
    } mma_req_logging;
    typedef struct _mma_res_logging {
    } mma_res_logging;

    typedef struct _mma_req_status_process {
    } mma_req_status_process;
    typedef struct _mma_res_status_process {
    } mma_res_status_process;

    typedef union _mma_rr_what {
        mma_req_number_pid req_number_pid;
        mma_res_number_pid res_number_pid;
        mma_req_logging req_logging;
        mma_res_logging res_logging;
        mma_req_status_process req_status_process;
        mma_res_status_process res_status_process;
    } mma_rr_what;

    typedef struct _mma_rr {
        i32 command;
        mm_rr common;
        mma_rr_what what;
    } mma_rr;

    rcode initialize_app_message_queue_server();
    rcode initialize_app_message_queue_client();

    const c8* get_mma_rr_cmd_strings(i32 _command);
    mm_rr mm_rr_make(ID_event* _maker, i32 _receiver, bool _need_response, ID_event* _verifier);
    mm_rr* mm_rr_create(ID_event* _maker, i32 _receiver, bool _need_response, ID_event* _verifier);
    void mm_rr_destroy(mm_rr* _rrc);
    mma_rr mma_rr_make(i32 _command, mm_rr _rrc);
    mma_rr* mma_rr_create(i32 _command, mm_rr _rrc);
    void mma_rr_destroy(mma_rr* _rr);
    i32 get_command_correspondence(i32 command_mm);

    mq_info* get_mq_mq_info(i32 _number);

    rcode write_impotant_logging(mma_req_logging _logging);
    rcode process_message_queue(c8* _msg);

#ifdef __cplusplus
    }
#endif

#endif // ___MMM_APP_MQ_H___
