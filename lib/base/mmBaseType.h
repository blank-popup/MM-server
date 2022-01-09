#ifndef ___MM_BASE_BASETYPE_H___
#define ___MM_BASE_BASETYPE_H___

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <stddef.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <pwd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include "../mmLogSimple.h"


#ifdef __cplusplus
extern "C" {
#endif

    typedef char                c8;
    typedef signed char         i8;
    typedef unsigned char       u8;
    typedef short               i16;
    typedef unsigned short      u16;
    typedef int                 i32;
    typedef unsigned int        u32;
    typedef long                i64;
    typedef unsigned long       u64;
    typedef float               f32;
    typedef double              f64;
    typedef long double         f128;

#define MM_MESSAGE_UNREACHABLE_CODE "Oops! This is unreachable code"

#define MM_SIZE_PATH_RELATIVE (256)
#define MM_SIZE_NAME (64)
#define MM_SIZE_PATH (1024)
#define MM_SIZE_YYYYMMDD (9)
#define MM_SIZE_HHMMSS (7)

    typedef i32 rcode;

    typedef enum _rcode_value {
        // ack_pass_request = 1,
        ack = 0,
        nak = -1,
    } rcode_value;

    typedef rcode(*action_return)(rcode, const c8*, const c8*, const c8*, i32);

    rcode areturn(rcode _rc, action_return _ar, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line);

#ifndef MAX
#define MAX(a, b)       ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)       ((a) < (b) ? (a) : (b))
#endif

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_BASETYPE_H___
