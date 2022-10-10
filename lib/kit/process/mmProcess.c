#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <linux/limits.h>
#include <stdbool.h>
#include "mmProcess.h"
#include "../../mmLogSimple.h"


void* process_malloc(size_t _size)
{
    void* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}

int process_check_pid_status(pid_t _pid, const char* _name_partial)
{
    if (_pid <= 0 || _name_partial == NULL) {
        return PROCESS_STATUS_UNKNOWN;
    }

    char path_proc[PATH_MAX] = { 0 };
    memset(path_proc, 0x00, sizeof(path_proc));
    snprintf(path_proc, sizeof(path_proc), "/proc/%d/cmdline", _pid);
    FILE* fp = fopen(path_proc, "r");
    if (fp != NULL) {
        char name_proc[PATH_MAX] = { 0 };
        fread(name_proc, sizeof(char), PATH_MAX, fp);
        fclose(fp);

        if (strstr(name_proc, _name_partial) != NULL) {
            return PROCESS_STATUS_ACITVE;
        }
        else {
            return PROCESS_STATUS_REVIVING;
        }
    }
    else {
        return PROCESS_STATUS_NOT_EXIST;
    }
}

int process_daemonize(pid_t* _pid_daemon, action_run _run, const char* _filepath, int _argc, char* _argv[], const char* _path, const char* _infile, const char* _outfile, const char* _errfile)
{
    if (_pid_daemon == NULL) {
        return -1;
    }
    if (_path == NULL) {
        _path = DAEMON_DEFAULT_PATH;
    }
    if (_infile == NULL) {
        _infile= DAEMON_DEFAULT_STANDARD_IO;
    }
    if (_outfile == NULL) {
        _outfile = DAEMON_DEFAULT_STANDARD_IO;
    }
    if (_errfile == NULL) {
        _errfile = DAEMON_DEFAULT_STANDARD_IO;
    }

    int fd0_pipe[2];
    pipe(fd0_pipe);
    pid_t pid = fork();

    if (pid > 0) {
        close(fd0_pipe[1]);
        read(fd0_pipe[0], _pid_daemon, sizeof(*_pid_daemon));
    } else if (pid == 0) {
        close(fd0_pipe[0]);
        if (setsid() < 0) {
            exit(EXIT_FAILURE);
        }

        signal(SIGCHLD, SIG_IGN);
        signal(SIGHUP, SIG_IGN);

        int fd1_pipe[2];
        pipe(fd1_pipe);
        pid = fork();
        if (pid > 0) {
            close(fd1_pipe[1]);
            read(fd1_pipe[0], _pid_daemon, sizeof(*_pid_daemon));
            write(fd0_pipe[1], _pid_daemon, sizeof(*_pid_daemon));
            exit(EXIT_SUCCESS);
        } else if (pid == 0) {
            close(fd1_pipe[0]);
            *_pid_daemon = getpid();
            write(fd1_pipe[1], _pid_daemon, sizeof(*_pid_daemon));

            umask(0);
            chdir(_path);

            int fd;
            for (fd = sysconf(_SC_OPEN_MAX); fd >= 0; --fd) {
                close(fd);
            }

            stdin = fopen(_infile, "r");
            stdout = fopen(_outfile, "w+");
            stderr = fopen(_errfile, "w+");

            char* path = getenv("PATH");
            size_t length_path = strlen(path);
            size_t length_path_new = strlen(_argv[4]) + 1 + strlen(path) + 1;
            char path_new[length_path_new];
            memset(path_new, 0x00, length_path_new);
            sprintf(path_new, "%s:%s", _argv[4], path);
            setenv("PATH", path_new, 1);

            if (_run != NULL) {
                _run((char*)_filepath, _argv);
            }
        } else {
            exit(EXIT_FAILURE);
        }
    } else {
        exit(EXIT_FAILURE);
    }

    return 0;
}

FILE* process_popen_python(const char* _python, const char* _args, bool _stderr)
{
    if (_python == NULL) {
        return NULL;
    }

    size_t length = strlen(PROCESS_COMMAND_PYTHON);
    length += 1 + strlen(_python);
    if (_args != NULL) {
        length += 1 + strlen(_args);
    }
    if (_stderr == true) {
        length += 1 + strlen(PROCESS_REDIRECT_STDERR_TO_STDOUT);
    }

    char* command = memloc(length + 1);
    snprintf(command, length + 1, "%s %s", PROCESS_COMMAND_PYTHON, _python);
    if (_args != NULL) {
        snprintf(command + strlen(command), length + 1 - strlen(command), " %s", _args);
    }
    if (_stderr == true) {
        snprintf(command + strlen(command), length + 1 - strlen(command), " %s", PROCESS_REDIRECT_STDERR_TO_STDOUT);
    }

    FILE* file = popen(command, PROCESS_MODE_READ);
    memfree(command);

    return file;
}

char* process_popen_fgets(FILE* _file, size_t _size_buffer)
{
    if (_file == NULL) {
        return NULL;
    }

    char* buffer = memloc(_size_buffer);
    size_t size_reading = _size_buffer;
    char* readings = memloc(size_reading);
    while (fgets(buffer, _size_buffer, _file)) {
        if (strlen(buffer) > size_reading - 1 - strlen(readings)) {
            size_reading *= 2;
            char* readings_new = memloc(size_reading);
            snprintf(readings_new, size_reading, "%s", readings);
            memfree(readings);
            readings = readings_new;
        }
        snprintf(readings + strlen(readings), size_reading - strlen(readings), "%s", buffer);
        memset(buffer, 0x00, _size_buffer);
    }
    memfree(buffer);

    return readings;
}