#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <pwd.h>
#include "mmLogSimple.h"


void LogSimple(int _level, const char* _filepath, int _line, pid_t _pid, pthread_t _tid, const char* _message, ...)
{
    struct passwd *pw = getpwuid(getuid());
    const char* directory_home = pw->pw_dir;
    size_t length_filename = strlen("mmm_yyyymmdd.log");
    char logdirectory[MM_SIZE_PATH_LOG_SIMPLE - length_filename];
    memset(logdirectory, 0x00, sizeof(logdirectory));
    sprintf(logdirectory, "%s/%s", directory_home, MM_DIRECTORY_LOG_SIMPLE);
    mkdir(logdirectory, 0775);

    struct timeval val;
    struct tm* t = NULL;
    gettimeofday(&val, NULL);
    t = localtime(&val.tv_sec);

    char logfilepath[MM_SIZE_PATH_LOG_SIMPLE] = { 0, };
    memset(logfilepath, 0x00, sizeof(logfilepath));
    sprintf(logfilepath, "%smmm_%04d%02d%02d.log", logdirectory, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    char logline[MM_SIZE_LINE_LOG_SIMPLE];
    memset(logline, 0x00, sizeof(logline));
    sprintf(logline, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, val.tv_usec);
    sprintf(logline + strlen(logline), " [Lv: %d] (%d:%lu)", _level, _pid, _tid);
    va_list argp;
    va_start(argp, _message);
    vsprintf(logline + strlen(logline), _message, argp);
    va_end(argp);
    sprintf(logline + strlen(logline), " (%s : %d)\n", _filepath, _line);

    FILE* fp = fopen(logfilepath, "a");
    fputs(logline, fp);
    fclose(fp);
}
