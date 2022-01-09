#include <fcntl.h>
#include "mmEtc.h"


double convert_pointer_to_double(void* _pointer)
{
    unsigned long address = (unsigned long)_pointer;

    return (double)address;
}

void* convert_double_to_pointer(double _double)
{
    unsigned long address = (unsigned long)_double;

    return (void*)address;
}

size_t get_index_from_array(void* _pointer_first_item, void* _pointer_item, size_t _size_item)
{
    return ((char*)_pointer_item - (char*)_pointer_first_item) / _size_item;
}

bool fd_is_valid(int _fd)
{
    return fcntl(_fd, F_GETFD) != -1;
}


long pack_two_int(int _upper, int _lower)
{
    return ((long)_upper) << 32 | _lower;
}

int unpack_upper_long(long _pack)
{
    return (int)((_pack & 0xFFFFFFFF00000000LL) >> 32);
}

int unpack_lower_long(long _pack)
{
    return (int)(_pack & 0x00000000FFFFFFFFLL);
}
