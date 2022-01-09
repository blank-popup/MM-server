#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "mmTime.h"


char* time_get_current_micro_localdatetime()
{
    struct timeval val;
    struct tm* t = NULL;

    gettimeofday(&val, NULL);
    t = localtime(&val.tv_sec);

    size_t length_time = 26;
    char* string_time = malloc(length_time + 1);
    if (string_time == NULL) {
        return NULL;
    }
    if (memset(string_time, 0x00, length_time + 1) != string_time) {
        free(string_time);
        return NULL;
    }

    sprintf(string_time, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, val.tv_usec);

    return string_time;
}

char* time_get_current_micro_UTCdatetime()
{
    struct timeval val;
    struct tm* t = NULL;

    gettimeofday(&val, NULL);
    t = gmtime(&val.tv_sec);

    size_t length_time = 26;
    char* string_time = malloc(length_time + 1);
    if (string_time == NULL) {
        return NULL;
    }
    if (memset(string_time, 0x00, length_time + 1) != string_time) {
        free(string_time);
        return NULL;
    }

    sprintf(string_time, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, val.tv_usec);

    return string_time;
}


int time_sleep_select(__time_t _sec, __suseconds_t _usec)
{
    struct timeval tv;
    tv.tv_sec = _sec;
    tv.tv_usec = _usec;

    if (select(0, NULL, NULL, NULL, &tv) < 0) {
        return -1;
    }

    return 0;
}


double time_convert_timeval_to_double(struct timeval _timeval)
{
    return _timeval.tv_sec + (double)_timeval.tv_usec / 1000000;
}

struct timeval time_convert_double_to_timeval(double _value)
{
    struct timeval value;
    value.tv_sec = (__time_t)_value;
    value.tv_usec = (__suseconds_t)((_value - value.tv_sec) * 1000000);

    return value;
}


struct timeval time_add_timeval(struct timeval _timeval0, struct timeval _timeval1)
{
    struct timeval value;
    value.tv_sec = _timeval0.tv_sec + _timeval1.tv_sec;
    value.tv_usec = _timeval0.tv_usec + _timeval1.tv_usec;
    if (value.tv_usec >= 1000000) {
        value.tv_sec += 1;
        value.tv_usec -= 1000000;
    }

    return value;
}

struct timeval time_sub_timeval(struct timeval _timeval0, struct timeval _timeval1)
{
    struct timeval value;
    value.tv_sec = _timeval0.tv_sec - _timeval1.tv_sec;
    value.tv_usec = _timeval0.tv_usec - _timeval1.tv_usec;
    if (value.tv_usec < 0) {
        value.tv_sec -= 1;
        value.tv_usec += 1000000;
    }

    return value;
}
