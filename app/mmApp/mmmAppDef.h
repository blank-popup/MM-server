#ifndef ___MMM_APP_DEF_H___
#define ___MMM_APP_DEF_H___

#include "mmmAppType.h"
#include "mmmAppJson.h"
#include "mmmAppSOCK.h"
#include "mmmAppSHM.h"
#include "mmmAppMQ.h"
#include "mmmAppPipe.h"


#ifdef __cplusplus
extern "C" {
#endif

#define PROCESS_TYPE_LOGGER "Logger"
#define PROCESS_TYPE_DISPATCHER "Dispatcher"
#define PROCESS_TYPE_SERVER "Server"
#define PROCESS_TYPE_WORKER "Worker"

#define DIRNAME_DEFAULT_CONFIG "config"
#define FilENAME_DEFAULT_CONFIG "mm.json"
#define FilENAME_DEFAULT_LOGGER "logger.json"
#define FilENAME_DEFAULT_IMMANENCE "immanence.json"

#define LOGGING_START_MARK "__________________"
#define LOGGING_HEAD_MARK "_____"

#define NAME_MM "mm"
#define NAME_DIRECTORY_ROOT "directory_root"
#define NAME_FILENAME_MAIN "filename_main"
#define NAME_DIRECTORY_PYTHON "directory_python"
#define NAME_FILEPATH_LOG "filepath_log"
#define NAME_PROCESS_LOGGER "process_logger"
#define NAME_NUMBER_PROCESS_LOGGER "number_process_logger"
#define NAME_LOGGER "logger"
#define NAME_TAG "tag"


#define NAME_REST "REST"

#define LOGGER_TAG_APP "app"
#define LOGGER_TAG_SO "so"

    // mmmApp.c

    // mmmAppTask.c
    void run_daemon(c8* _filepath, c8* _argv[]);
    int run_process(int _argc, char* _argv[]);

    rcode set_theApp_config(const c8* _filepath_mm_json);
    rcode free_theApp_config();
    rcode set_theApp_tag(i32 _number);
    rcode set_theApp_path(i32 _argc, c8* _argv[]);
    rcode set_app_action_in_return();
    rcode set_theApp_logger(const c8* _directory_root, const c8* _filepath_log, i32 _type, i32 _ith);

    rcode initialize_mmApp(JSON_array* _array_sub_thread, i32 _type, i32 _ith);
    rcode set_theApp_tag_pid();

    rcode set_app_control_teminate(bool _able);
    rcode get_app_control_teminate(bool* _able);
    rcode generate_app_control_id(size_t* _id);
    i32 process_check_pid_status(pid_t pid, const c8* _name_partial);
    bool check_have_to_revive_process_next();
    void revive_process(i32 _number);
    void run_periodically();

    void handler_EXODUS(i32 _sig);
    void handler_SIGPIPE(i32 _sig);
    void handler_SIGUSR1(i32 _sig);
    void handler_SIGUSR2(i32 _sig);

#ifdef __cplusplus
    }
#endif

#endif // ___MMM_APP_DEF_H___
