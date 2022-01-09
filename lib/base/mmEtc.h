#ifndef ___MM_BASE_ETC_H___
#define ___MM_BASE_ETC_H___

#include <stddef.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t)&(((TYPE*)0)->MEMBER))
#endif
#ifndef container_of
#define container_of(PTR, TYPE, MEMBER) ((TYPE*)((char*)(PTR) - offsetof(TYPE, MEMBER)))
#endif


    double convert_pointer_to_double(void* _pointer);
    void* convert_double_to_pointer(double _double);
    size_t get_index_from_array(void* _pointer_first_item, void* _pointer_item, size_t _size_item);
    bool fd_is_valid(int _fd);

    long pack_two_int(int _upper, int _lower);
    int unpack_upper_long(long _pack);
    int unpack_lower_long(long _pack);

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_ETC_H___
