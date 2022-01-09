#ifndef ___MM_BASE_MEMORY_H___
#define ___MM_BASE_MEMORY_H___

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

// #define ___MM_USE_MEMORY_TRACER___

    void* memory_allocate(size_t _size);
#define memory_free(m) do { free(m); m = NULL; } while(0);

    void* memory_duplicate(void* _source, size_t _size);
    void* memory_reallocate(void* _source, size_t _capacity_old, size_t _capacity_new);
    void* memory_double_capacity(void* _source, size_t _capacity);

    void* memory_allocate_trace(size_t _size, char* _filepath, int _line);
#define memory_free_trace(m, f, l) do { memory_tracer_remove_item(m, f, l); free(m); m = NULL; } while(0);

    int memory_tracer_initialize();
    int memory_tracer_add_item(void* _memory, size_t _size, char* _filepath, int _line);
    int memory_tracer_remove_item(void* _memory, char* _filepath, int _line);
    int memory_tracer_print(const char* _title, const char* _prefix);
    int memory_tracer_destroy();

#ifdef ___MM_USE_MEMORY_TRACER___
#define memloc(s) memory_allocate_trace(s, __FILE__, __LINE__)
#define memfree(m) memory_free_trace(m, __FILE__, __LINE__)
#else
#define memloc(s) memory_allocate(s)
#define memfree(m) memory_free(m)
#endif

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_MEMORY_H___
