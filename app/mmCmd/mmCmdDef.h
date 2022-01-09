#ifndef ___MM_CMD_DEF_H___
#define ___MM_CMD_DEF_H___

#include "../mmApp/mmmAppExt.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LOG(...) do { printf(__VA_ARGS__); printf("\n"); } while(0)
#define LOGI(...) do { fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); } while(0)
#define LOGE(...) do { fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); } while(0)

    void print_directory_current();
    void set_config();
    void send_mq(i32 _command, i32 _target, i32 _argc, c8* _argv[]);

#ifdef __cplusplus
}
#endif

#endif // ___MM_CMD_DEF_H___
