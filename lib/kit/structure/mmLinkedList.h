#ifndef ___MM_KIT_STRUCTURE_LINKEDLIST_H___
#define ___MM_KIT_STRUCTURE_LINKEDLIST_H___

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _linked_list_item linked_list_item;
    typedef struct _linked_list linked_list;

    typedef bool(*linked_list_item_destroy_custom)(linked_list_item*, void*);

    void* linked_list_malloc(size_t _size);
#define linked_list_free(m) do { free(m); m = NULL; } while(0);

#ifndef memloc
#define memloc(s) linked_list_malloc(s)
#endif
#ifndef memfree
#define memfree(m) linked_list_free(m)
#endif

    linked_list_item* linked_list_item_get_next(linked_list_item* _item);
    linked_list_item* linked_list_item_get_previous(linked_list_item* _item);
    linked_list_item* linked_list_get_head(linked_list* _ll);
    linked_list_item* linked_list_get_tail(linked_list* _ll);

    linked_list* linked_list_create();
    linked_list_item* linked_list_item_create(void* _data, size_t _size_data);
    bool linked_list_item_destroy(linked_list_item* _item, linked_list_item_destroy_custom _destroy, void* _arg);
    bool linked_list_destroy(linked_list* _ll, linked_list_item_destroy_custom _destroy, void* _arg);

    linked_list* linked_list_append_item(linked_list* _ll, linked_list_item* _item);
    linked_list* linked_list_insert_item(linked_list* _ll, linked_list_item* _item, size_t _ith);
    size_t linked_list_get_count(linked_list* _ll);
    linked_list_item* linked_list_get_ith(linked_list* _ll, size_t _ith);
    bool linked_list_include_item(linked_list* _ll, linked_list_item* _item);
    linked_list_item* linked_list_pop_ith(linked_list* _ll, size_t _ith);
    linked_list* linked_list_remove_item(linked_list* _ll, linked_list_item* _item, linked_list_item_destroy_custom _destroy, void* _arg);
    linked_list* linked_list_remove_ith(linked_list* _ll, size_t _ith, linked_list_item_destroy_custom _destroy, void* _arg);
    linked_list* linked_list_remove_all_item(linked_list* _ll, linked_list_item_destroy_custom _destroy, void* _arg);

    void* linked_list_item_get_data(linked_list_item* _item);
    linked_list_item* data_get_linked_list_item(void* _data);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_STRUCTURE_LINKEDLIST_H___
