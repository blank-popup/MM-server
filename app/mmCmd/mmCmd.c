#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "mmCmdExt.h"


int main(int _argc, char* _argv[])
{
    print_directory_current();
    set_config();

    c8* ipc = NULL;
    i32 command = -1;
    i32 target = -1;
    if (_argc == 4) {
        ipc = _argv[1];
        command = (i32)strtol(_argv[2], (c8**)NULL, 10);
        target = (i32)strtol(_argv[3], (c8**)NULL, 10);
    }
    else {
        ipc = "mq";
        command = 1;
        target = 0;
        // ipc = "sock";
        // command = 1;
        // target = 1;
    }
    LOGI("ipc: [%s], command: [%d], target: [%d]", ipc, command, target);

    if (strcmp(ipc, "mq") == 0) {
        i32 count_processes = (i32)get_mm_count_processes();
        if (target < count_processes) {
            send_mq(command, target, _argc, _argv);
        }
        else {
            for (i32 ii = 0; ii < count_processes; ++ii) {
                send_mq(command, ii, _argc, _argv);
            }
        }
    }
    else {
        fprintf(stderr, "mmCmd [sock <command> <target> | mq <command> <target>] message\n");
        return 1;
    }

    fprintf(stderr, "mmCmd finish\n");

    return 0;
}
