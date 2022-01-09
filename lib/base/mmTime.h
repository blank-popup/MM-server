#ifndef ___MM_BASE_TIME_H___
#define ___MM_BASE_TIME_H___

#include <sys/types.h>


#ifdef __cplusplus
extern "C" {
#endif

    char* time_get_current_micro_localdatetime();
    char* time_get_current_micro_UTCdatetime();

    int time_sleep_select(__time_t _sec, __suseconds_t _usec);

    double time_convert_timeval_to_double(struct timeval _timeval);
    struct timeval time_convert_double_to_timeval(double _value);

    struct timeval time_add_timeval(struct timeval _timeval0, struct timeval _timeval1);
    struct timeval time_sub_timeval(struct timeval _timeval0, struct timeval _timeval1);

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_TIME_H___
