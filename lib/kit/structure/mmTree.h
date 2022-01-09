#ifndef ___MM_KIT_STRUCTURE_TREE_H___
#define ___MM_KIT_STRUCTURE_TREE_H___

#include <stdbool.h>
#include <stdlib.h>


#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _tree_item tree_item;

    tree_item* tree_malloc(size_t _size);
#define tree_free(m) do { free(m); m = NULL; } while(0);

#ifndef memloc
#define memloc(s) tree_malloc(s)
#endif
#ifndef memfree
#define memfree(m) tree_free(m)
#endif

    tree_item* tree_item_get_parent(tree_item* _item);
    tree_item* tree_item_get_next_sibling(tree_item* _item);
    tree_item* tree_item_get_previous_sibling(tree_item* _item);
    tree_item* tree_item_get_head_child(tree_item* _item);
    tree_item* tree_item_get_tail_child(tree_item* _item);

    typedef bool(*tree_item_destroy_custom)(tree_item*, void*);

    tree_item* tree_item_create(void* _data, size_t _size_data);
    bool tree_item_destroy(tree_item* _item, tree_item_destroy_custom _destroy, void* _arg);

    tree_item* tree_item_append_child(tree_item* _parent, tree_item* _child);
    tree_item* tree_item_insert_child(tree_item* _parent, tree_item* _child, size_t _ith);
    tree_item* tree_item_get_child(tree_item* _parent, size_t _ith);
    bool tree_item_include_child(tree_item* _parent, tree_item* _child);
    bool tree_item_is_descendant(tree_item* _parent, tree_item* _child);
    tree_item* tree_item_remove_child(tree_item* _parent, tree_item* _child, tree_item_destroy_custom _destroy, void* _arg);
    tree_item* tree_item_remove_ith(tree_item* _parent, size_t _ith, tree_item_destroy_custom _destroy, void* _arg);
    tree_item* tree_item_remove_all_child(tree_item* _parent, tree_item_destroy_custom _destroy, void* _arg);

    void* tree_item_get_data(tree_item* _item);
    tree_item* data_get_tree_item(void* _data);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_STRUCTURE_TREE_H___
