#ifndef ___MM_KIT_STRUCTURE_CIRCULAR_LINKEDLIST_H___
#define ___MM_KIT_STRUCTURE_CIRCULAR_LINKEDLIST_H___

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _circular_linked_list_item circular_linked_list_item;

    typedef bool(*circular_linked_list_item_destroy_custom)(circular_linked_list_item*, void*);

    circular_linked_list_item* circular_linked_list_malloc(size_t _size);
#define circular_linked_list_free(m) do { free(m); m = NULL; } while(0);

#ifndef memloc
#define memloc(s) circular_linked_list_malloc(s)
#endif
#ifndef memfree
#define memfree(m) circular_linked_list_free(m)
#endif

    circular_linked_list_item* circular_linked_list_item_get_next(circular_linked_list_item* _item);
    circular_linked_list_item* circular_linked_list_item_get_previous(circular_linked_list_item* _item);

    circular_linked_list_item* circular_linked_list_item_create(void* _data, size_t _size_data);
    bool circular_linked_list_item_destroy(circular_linked_list_item* _item, circular_linked_list_item_destroy_custom _destroy, void* _arg);

    circular_linked_list_item* circular_linked_list_item_append_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target);
    circular_linked_list_item* circular_linked_list_item_insert_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target, size_t _ith);
    size_t circular_linked_list_item_get_count(circular_linked_list_item* _item);
    circular_linked_list_item* circular_linked_list_item_get_item(circular_linked_list_item* _item, size_t _ith);
    bool circular_linked_list_item_include_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target);
    circular_linked_list_item* circular_linked_list_item_remove_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target, circular_linked_list_item_destroy_custom _destroy, void* _arg);
    circular_linked_list_item* circular_linked_list_item_remove_ith(circular_linked_list_item* _item, size_t _ith, circular_linked_list_item_destroy_custom _destroy, void* _arg);
    // NEED to TEST !!!
    bool circular_linked_list_item_remove_all_item(circular_linked_list_item* _item, circular_linked_list_item_destroy_custom _destroy, void* _arg);

    void* circular_linked_list_item_get_data(circular_linked_list_item* _item);
    circular_linked_list_item* data_get_circular_linked_list_item(void* _data);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_STRUCTURE_CIRCULAR_LINKEDLIST_H___
