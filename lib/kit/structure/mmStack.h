#ifndef ___MM_KIT_STRUCTURE_STACK_H___
#define ___MM_KIT_STRUCTURE_STACK_H___

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _stack_item stack_item;
    typedef struct _stack stack;

    typedef bool(*stack_item_destroy_custom)(stack_item*, void*);

    stack* stack_malloc(size_t _size);
#define stack_free(m) do { free(m); m = NULL; } while(0);

#ifndef memloc
#define memloc(s) stack_malloc(s)
#endif
#ifndef memfree
#define memfree(m) stack_free(m)
#endif

    stack* stack_create();
    stack_item* stack_item_create(void* _data, size_t _size_data);
    bool stack_item_destroy(stack_item* _item, stack_item_destroy_custom _destroy, void* _arg);
    bool stack_destroy(stack* _s, stack_item_destroy_custom _destroy, void* _arg);

    stack* stack_push_item(stack* _s, stack_item* _item);
    stack_item* stack_pop_item(stack* _s);
    size_t stack_get_count(stack* _s);
    void* stack_item_get_data(stack_item* _item);
    stack_item* data_get_stack_item(void* _data);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_STRUCTURE_STACK_H___
