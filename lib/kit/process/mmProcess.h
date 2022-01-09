#ifndef ___MM_KIT_PROCESS_H___
#define ___MM_KIT_PROCESS_H___

#include <pthread.h>


#ifdef __cplusplus
extern "C" {
#endif

    void* process_malloc(size_t _size);
#define process_free(m) do { free(m); m = NULL; } while(0);

#ifndef memloc
#define memloc(s) process_malloc(s)
#endif
#ifndef memfree
#define memfree(m) process_free(m)
#endif

#define PROCESS_STATUS_UNKNOWN (-1)
#define PROCESS_STATUS_NOT_EXIST (0)
#define PROCESS_STATUS_ACITVE (1)
#define PROCESS_STATUS_REVIVING (2)

    int process_check_pid_status(pid_t _pid, const char* _name_partial);

#define DAEMON_DEFAULT_PATH "/"
#define DAEMON_DEFAULT_STANDARD_IO "/dev/null"

    typedef void(*action_run)(char*, char*[]);
    int process_daemonize(pid_t* _pid_daemon, action_run _run, const char* _filepath, int _argc, char* _argv[], const char* _path, const char* _infile, const char* _outfile, const char* _errfile);

#define PROCESS_COMMAND_PYTHON "python"
#define PROCESS_REDIRECT_STDERR_TO_STDOUT "2>&1"
#define PROCESS_MODE_READ "r"
#define PROCESS_MODE_WRITE "w"

    FILE* process_popen_python(const char* _python, const char* _args, bool _stderr);
    char* process_popen_fgets(FILE* _file, size_t _size_buffer);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_PROCESS_H___
