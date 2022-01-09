#include "mmmAppExt.h"


rcode initialize_app_message_queue_server()
{
    const c8* name = get_mmApp_self_message_queue_name();
    __syscall_slong_t maxmsg = get_mmApp_self_message_queue_maxmsg();
    __syscall_slong_t msgsize = get_mmApp_self_message_queue_msgsize();
    if (STRING_IS_NOT_EMPTY(name)) {
        theApp.sds.mq.number = theApp.self.number;
        theApp.sds.mq.maxmsg = maxmsg;
        theApp.sds.mq.msgsize = msgsize;
        if ((theApp.sds.mq.fd = mqueue_open(name, O_CREAT | O_RDWR, 0666, maxmsg, msgsize)) < 0) {
            lerror(l_app, "Fail to initialize message queue server name[%s]", name);
            return mma(nak);
        }
        ldebug(l_app, "Initialized message queue server name[%s] fd[%d]", name, theApp.sds.mq.fd);

        if (epoll_add_event(theApp.sds.ep, theApp.sds.mq.fd, EPOLLIN | EPOLLEXCLUSIVE) < 0) {
            lerror(l_app, "Fail to add event of message queue server name[%s] fd[%d]", name, theApp.sds.mq.fd);
            return mma(nak);
        }
        linfo(l_app, "Added event of message queue server name[%s] fd[%d]", name, theApp.sds.mq.fd);

        return mma(ack);
    }

    return mma(ack);
}

rcode initialize_app_message_queue_client()
{
    theApp.cds.mq.list = linked_list_create();
    theApp.cds.mq.size = get_mm_count_processes();
    for (size_t ii = 0; ii < theApp.cds.mq.size; ++ii) {
        linked_list_item* item = linked_list_item_create(NULL, sizeof(mq_info));
        mq_info* info = linked_list_item_get_data(item);
        tag_process tag = process_number_get_tag((i32)ii);
        const c8* name = get_mmApp_message_queue_name(tag.type, tag.ith);
        __syscall_slong_t maxmsg = get_mmApp_message_queue_maxmsg(tag.type);
        __syscall_slong_t msgsize = get_mmApp_message_queue_msgsize(tag.type);

        while (true) {
            info->number = tag.number;
            if ((info->fd = mqueue_open(name, O_CREAT | O_RDWR, 0666, maxmsg, msgsize)) < 0) {
                ldebug(l_app, "%s", "Fail to open message queue name[%s]", name);
                time_sleep_select(0, 100000);
                continue;
            }
            info->maxmsg = maxmsg;
            info->msgsize = msgsize;
            linked_list_append_item(theApp.cds.mq.list, item);
            break;
        }
        ldebug (l_app, "Initialized message queue[%u] client fd[%d] open maxmsg, msgsize: [%ld, %ld]", info->number, info->fd, info->maxmsg, info->msgsize);
    }
    linfo(l_app, "%s", "Initialized message queue all client");

    return mma(ack);
}


static const c8* ___mma_req_cmd_strings[] = MMA_REQ_CMD_STRINGS;
static const c8* ___mma_res_cmd_strings[] = MMA_RES_CMD_STRINGS;

const c8* get_mma_rr_cmd_strings(i32 _command)
{
    if (_command >= MMA_REQ_BASE && _command < MMA_RES_BASE) {
        return ___mma_req_cmd_strings[_command - MMA_REQ_BASE];
    }
    else if (_command >= MMA_RES_BASE && _command < MMP_REQ_BASE) {
        return ___mma_res_cmd_strings[_command - MMA_RES_BASE];
    }
    else {
        return NULL;
    }
}

mm_rr mm_rr_make(ID_event* _maker, i32 _receiver, bool _need_response, ID_event* _verifier)
{
    mm_rr rrc;
    rrc.sender.number = theApp.self.number;
    generate_app_control_id(&rrc.sender.id);
    if (_maker != NULL) {
        rrc.maker = *_maker;
    }
    else {
        rrc.maker = rrc.sender;
    }
    rrc.receiver = _receiver;
    rrc.need_response = _need_response;
    gettimeofday(&rrc.when, NULL);
    if (_verifier != NULL) {
        rrc.verifier = *_verifier;
    }
    else {
        rrc.verifier.number = -1;
        rrc.verifier.id = -1;
    }

    return rrc;
}

mm_rr* mm_rr_create(ID_event* _maker, i32 _receiver, bool _need_response, ID_event* _verifier)
{
    mm_rr* rrc = memloc(sizeof(mm_rr));
    if (rrc == NULL) {
        return NULL;
    }
    if (_maker != NULL) {
        rrc->maker = *_maker;
    }
    rrc->sender.number = theApp.self.number;
    generate_app_control_id(&rrc->sender.id);
    rrc->receiver = _receiver;
    rrc->need_response = _need_response;
    gettimeofday(&rrc->when, NULL);
    if (_verifier != NULL) {
        rrc->verifier = *_verifier;
    }

    return rrc;
}

void mm_rr_destroy(mm_rr* _rrc)
{
    if (_rrc != NULL) {
        memfree(_rrc);
    }
}

mma_rr mma_rr_make(i32 _command, mm_rr _rrc)
{
    mma_rr rr;
    rr.command = _command;
    rr.common = _rrc;

    return rr;
}

mma_rr* mma_rr_create(i32 _command, mm_rr _rrc)
{
    mma_rr* rr = memloc(sizeof(mma_rr));
    if (rr == NULL) {
        return NULL;
    }
    rr->command = _command;
    rr->common = _rrc;

    return rr;
}

void mma_rr_destroy(mma_rr* _rr)
{
    if (_rr != NULL) {
        memfree(_rr);
    }
}

i32 get_command_correspondence(i32 command_mm)
{
    if (command_mm >= MMP_REQ_BASE && command_mm < MMP_RES_BASE) {
        return command_mm + MM_SIZE_RR_PLUGIN;
    }
    else if (command_mm >= MMP_RES_BASE && command_mm < (MMP_RES_BASE + MM_SIZE_RR_PLUGIN)) {
        return command_mm - MM_SIZE_RR_PLUGIN;
    }
    else if (command_mm >= MMA_REQ_BASE && command_mm < MMA_RES_BASE) {
        return command_mm + MM_SIZE_RR_APP;
    }
    else if (command_mm >= MMA_RES_BASE && command_mm < MMP_REQ_BASE) {
        return command_mm - MM_SIZE_RR_APP;
    }

    return MM_RR_ERROR;
}


mq_info* get_mq_mq_info(i32 _number)
{
    for (size_t ii = 0; ii < theApp.cds.mq.size; ++ ii) {
        linked_list_item* item = linked_list_get_ith(theApp.cds.mq.list, ii);
        if (item == NULL) {
            return NULL;
        }
        mq_info* info = linked_list_item_get_data(item);
        if (info != NULL && _number == info->number) {
            return info;
        }
    }

    return NULL;
}


static rcode process_mma_req_ask_number_pid(c8* _msg)
{
    if (_msg == NULL) {
        return mma(nak);
    }

    mma_rr* req = (mma_rr*)_msg;
    mma_req_number_pid ask_number_pid = req->what.req_number_pid;

    mq_info* info = get_mq_mq_info(req->common.sender.number);
    mm_rr rrc = mm_rr_make(&req->common.maker, req->common.sender.number, false, &req->common.sender);
    mma_rr res = mma_rr_make(get_command_correspondence(req->command), rrc);
    res.what.res_number_pid.pid = theApp.self.pid;
    mqueue_send(info->fd, &res, sizeof(mma_rr), 0);
    ldebug(l_app, "Send message [%s] to number[%d]", get_mma_rr_cmd_strings(res.command), req->common.sender);
    return mma(ack);
}

static rcode process_mma_res_ask_number_pid(c8* _msg)
{
    if (_msg == NULL) {
        return mma(nak);
    }

    mma_rr* res = (mma_rr*)_msg;
    mma_res_number_pid ask_next_information = res->what.res_number_pid;

    theApp.next.pid = ask_next_information.pid;
    linfo(l_app, "Set next pid: [%d]", theApp.next.pid);

    return mma(ack);
}


static const c8* ___logging_event_strings[] = LOGGING_EVENT_STRINGS;

static rcode process_mma_req_logging(c8* _msg)
{
    if (_msg == NULL) {
        return mma(nak);
    }

    mma_rr* req = (mma_rr*)_msg;
    mma_req_logging logging = req->what.req_logging;

    switch (logging.event) {
        case LOGGING_EVENT_PROCESS_TERMINATED:
            mmlog(l_app, logging.level, "%s: number[%d], pid[%d]", ___logging_event_strings[logging.event], logging.number, logging.pid);
            break;
        case LOGGING_EVENT_PROCESS_REVIVED:
            mmlog(l_app, logging.level, "%s: number[%d], pid[%d]", ___logging_event_strings[logging.event], logging.number, logging.pid);
            break;
        default:
            return mma(nak);
    }

    return mma(ack);
}

static rcode process_mma_res_logging(c8* _res)
{
    if (_res == NULL) {
        return mma(nak);
    }

    mma_rr* res = (mma_rr*)_res;
    mma_res_logging logging = res->what.res_logging;

    return mma(ack);
}

rcode write_impotant_logging(mma_req_logging _logging)
{
    i32 number = get_mm_number_process_logger();
    mq_info* info = get_mq_mq_info(number);
    mm_rr rrc = mm_rr_make(NULL, theApp.self.number, false, NULL);
    mma_rr req = mma_rr_make(MMA_REQ_LOGGING, rrc);
    req.what.req_logging = _logging;
    mqueue_send(info->fd, &req, sizeof(mma_rr), 0);

    return mma(ack);
}

rcode process_message_queue(c8* _msg)
{
    if (_msg == NULL) {
        return mma(nak);
    }

    i32 command = *(i32*)_msg;
    rcode rc;
    if (command >= MMA_REQ_BASE && command < (MMP_REQ_BASE)) {
        mma_rr* rr = (mma_rr*)_msg;
        ID_event* maker = &rr->common.maker;
        ID_event* sender = &rr->common.sender;
        linfo(l_app, LOGGING_HEAD_MARK " Get NEW Event[MQ] " LOGGING_HEAD_MARK " command(mma): [%d:%s], maker: [%lu:%d], sender: [%lu:%d]", command, get_mma_rr_cmd_strings(command), maker->id, maker->number, sender->id, sender->number);

        if (command == MMA_REQ_NUMBER_PID) {
            rc = process_mma_req_ask_number_pid(_msg);
        }
        else if (command == MMA_RES_NUMBER_PID) {
            rc = process_mma_res_ask_number_pid(_msg);
        }

        else if (command == MMA_REQ_LOGGING) {
            rc = process_mma_req_logging(_msg);
        }
        else if (command == MMA_RES_LOGGING) {
            rc = process_mma_res_logging(_msg);
        }

        else {
            lerror(l_app, "Command(mma) not programed: [%d], rcode: [-1]", command);
            return(nak);
        }

        ldebug(l_app, "Command maker: [%lu:%d], sender: [%lu:%d], rcode: [%d]", maker->id, maker->number, sender->id, sender->number, rc);
    }
    else {
        lerror(l_app, "Invalid command: [%d], rcode: [-1]", command);
        return(nak);
    }

    return mma(rc);
}
