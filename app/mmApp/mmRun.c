#include "mmRunExt.h"


int main(int _argc, char* _argv[])
{
    c8 arg_json[MM_SIZE_PATH] = { 0, };
    if (_argc == 1) {
        string_memset_print(arg_json, sizeof(arg_json), "%s/%s", DIRNAME_DEFAULT_CONFIG, FilENAME_DEFAULT_CONFIG);
    }
    else {
        string_memset_print(arg_json, sizeof(arg_json), "%s", _argv[1]);
    }
    fprintf(stderr, "Filepath json: [%s]\n", arg_json);

    JSON_value* value_config = json_parse_file(arg_json);
    JSON_object* object_config = json_value_get_object(value_config);
    if (object_config == NULL) {
        fprintf(stderr, "Not found %s\n", arg_json);
        return 1;
    }

    JSON_object* mm = json_object_get_object(object_config, NAME_MM);
    const c8* directory_root = json_object_get_string(mm, NAME_DIRECTORY_ROOT);
    fprintf(stderr, "Directory root: [%s]\n", directory_root);

    i32 count_process = 0;
    JSON_object* mmApps = json_object_get_object(object_config, NAME_MM_APPS);
    for (size_t ii = 0; ii < json_object_get_length(mmApps); ++ii) {
        const c8* name = json_object_get_name(mmApps, ii);
        JSON_object* mmApp = json_object_get_object(mmApps, name);
        size_t count_mmApp = (size_t)json_object_get_real_number(mmApp, NAME_COUNT_PROCESS);
        count_process += count_mmApp;
    }
    fprintf(stderr, "Count of process: [%d]\n", count_process);

    const c8* filename_main = json_object_get_string(mm, NAME_FILENAME_MAIN);
    fprintf(stderr, "Filename of main: [%s]\n", filename_main);

    const c8* directory_python = json_object_get_string(mm, NAME_DIRECTORY_PYTHON);
    fprintf(stderr, "Directory of python: [%s]\n", directory_python);

    pid_t pid = getpid();
    for (i32 ii = 0; ii < count_process; ++ii) {
        if (pid != getpid()) {
            break;
        }
        pid_t pid_daemon;
        c8 number[MM_SIZE_NAME] = { 0 };
        string_memset_print(number, sizeof(number), "%d", ii);
        c8* args_daemon[] = { (c8*)filename_main, (c8*)directory_root, arg_json, number, (c8*)directory_python, NULL };
        i32 size_args_daemon = 6;
        c8 filepath[MM_SIZE_PATH] = { 0 };
        string_memset_print(filepath, sizeof(filepath), "%s/%s", directory_root, filename_main);
        process_daemonize(&pid_daemon, run_daemon, filepath, size_args_daemon, args_daemon, NULL, NULL, NULL, NULL);
        fprintf(stderr, "Daemonized number: [%d] pid: [%d]\n", ii, pid_daemon);
    }

    json_value_free(value_config);

    return 0;
}
