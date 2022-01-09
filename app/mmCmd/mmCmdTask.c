#include "mmCmdExt.h"


void print_directory_current()
{
    c8 directory_current[MM_SIZE_PATH] = { 0, };
    getcwd(directory_current, sizeof(directory_current));
    LOGI("Current Directory: [%s]", directory_current);
}

void set_config()
{
    set_theApp_config("../mmApp/mm.json");
    LOGI("root value: [%p], common object: [%p]", (void*)theApp.config_root_value, (void*)theApp.common_object);
}

void send_mq(i32 _command, i32 _target, i32 _argc, c8* _argv[])
{
    mq_info mqi;
    LOGI("%s", "connect via message queue");
    tag_process tag = process_number_get_tag(_target);
    const c8* name = get_mmApp_message_queue_name(tag.type, tag.ith);
    LOGI("message queue name: [%s]", name);

    __syscall_slong_t maxmsg = get_mmApp_message_queue_maxmsg(tag.type);
    __syscall_slong_t msgsize = get_mmApp_message_queue_msgsize(tag.type);

    LOGE("maxmsg, msgsize: [%ld, %lu]", maxmsg, mqi.msgsize);
    if ((mqi.fd = mqueue_open(name, O_WRONLY, 0666, maxmsg, msgsize)) < 0) {
        LOGE("cannot open mqueue");
        return;
    }
    LOGI("opened mqueue");

    if ((_argc < 0 && _argc >= 0) || (_argv == NULL && _argv != NULL)) {
        return;
    }

    LOGE("Size of mma_req: [%lu]", sizeof(mma_rr));

    mma_rr req = { 0 };
    *(i32*)&req = _command;
    req.common.maker.number = 999;
    req.common.maker.id = 1818;
    req.common.sender.number = strtol(_argv[4], NULL, 10);
    req.common.sender.id = 7777;
    req.common.receiver = _target;

    if (mqueue_send(mqi.fd, (c8*)&req, sizeof(req), 0) < 0) {
        LOGE("cannot send message");
        return;
    }

    LOGI("sent message: command[%d] target[%d]", _command, _target);
}
