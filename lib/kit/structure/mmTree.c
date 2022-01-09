#include <string.h>
#include "mmTree.h"
#include "../../mmLogSimple.h"


struct _tree_item {
    struct _tree_item* parent;
    struct _tree_item* next_sibling;
    struct _tree_item* previous_sibling;
    struct _tree_item* head_child;
    struct _tree_item* tail_child;
};


tree_item* tree_malloc(size_t _size)
{
    tree_item* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}


tree_item* tree_item_get_parent(tree_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->parent;
}

tree_item* tree_item_get_next_sibling(tree_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->next_sibling;
}

tree_item* tree_item_get_previous_sibling(tree_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->previous_sibling;
}

tree_item* tree_item_get_head_child(tree_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->head_child;
}

tree_item* tree_item_get_tail_child(tree_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->tail_child;
}

tree_item* tree_item_create(void* _data, size_t _size_data)
{
    if (_size_data == 0) {
        return NULL;
    }

    tree_item* item = (tree_item*)memloc(sizeof(tree_item) + _size_data);
    if (_data != NULL) {
        memcpy(item + 1, _data, _size_data);
    }
    return item;
}

bool tree_item_destroy(tree_item* _item, tree_item_destroy_custom _destroy, void* _arg)
{
    if (_destroy != NULL) {
        return _destroy(_item, _arg);
    }

    if (_item == NULL) {
        return false;
    }

    memfree(_item);

    return true;
}


tree_item* tree_item_append_child(tree_item* _parent, tree_item* _child)
{
    if (_parent == NULL || _child == NULL) {
        return NULL;
    }

    if (_parent->head_child == NULL) {
        _parent->head_child = _child;
        _parent->tail_child = _child;
    }
    else {
        _parent->tail_child->next_sibling = _child;
        _child->previous_sibling = _parent->tail_child;
        _parent->tail_child = _child;
    }

    _child->parent = _parent;

    return _child;
}

tree_item* tree_item_insert_child(tree_item* _parent, tree_item* _child, size_t _ith)
{
    if (_parent == NULL || _child == NULL) {
        return NULL;
    }

    tree_item* child = tree_item_get_child(_parent, _ith);
    if (child == NULL) {
        return NULL;
    }
    tree_item* child_previous = child->previous_sibling;
    tree_item* child_next = child;

    if (child_previous == NULL) {
        _parent->head_child = _child;
    }
    else {
        child_previous->next_sibling = _child;
        _child->previous_sibling = child_previous;
    }
    if (child_next == NULL) {
        _parent->tail_child = _child;
    }
    else {
        child_next->previous_sibling = _child;
        _child->next_sibling = child_next;
    }

    _child->parent = _parent;

    return _child;
}

tree_item* tree_item_get_child(tree_item* _parent, size_t _ith)
{
    if (_parent == NULL) {
        return NULL;
    }

    tree_item* child = _parent->head_child;
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (child == NULL) {
            return NULL;
        }
        child = child->next_sibling;
    }

    return child;
}

bool tree_item_include_child(tree_item* _parent, tree_item* _child)
{
    if (_parent == NULL || _child == NULL) {
        return NULL;
    }

    if (_child->parent == _parent) {
        return true;
    }

    return false;
}

bool tree_item_is_descendant(tree_item* _parent, tree_item* _child)
{
    if (_parent == NULL || _child == NULL) {
        return NULL;
    }

    tree_item* parent = _child->parent;
    while (parent != NULL) {
        if (parent == _parent) {
            return true;
        }
        parent = parent->parent;
    }

    return false;
}

static tree_item* tree_item_remove_selected_child(tree_item* _parent, tree_item* _child, tree_item_destroy_custom _destroy, void* _arg)
{
    if (_parent == NULL || _child == NULL) {
        return NULL;
    }

    tree_item* child_previous = _child->previous_sibling;
    tree_item* child_next = _child->next_sibling;

    if (_child->head_child == NULL) {
        if (child_previous == NULL) {
            _parent->head_child = _parent->head_child->next_sibling;
        }
        else {
            child_previous->next_sibling = child_next;
        }
        if (child_next == NULL) {
            _parent->tail_child = _parent->tail_child->previous_sibling;
        }
        else {
            child_next->previous_sibling = child_previous;
        }
    }
    else {
        if (child_previous == NULL) {
            _parent->head_child = _child->head_child;
        }
        else {
            child_previous->next_sibling = _child->head_child;
            _child->head_child->previous_sibling = child_previous;
        }
        if (child_next == NULL) {
            _parent->tail_child = _child->tail_child;
        }
        else {
            child_next->previous_sibling = _child->tail_child;
            _child->tail_child->next_sibling = child_next;
        }

        tree_item* grandchild = _child->head_child;
        while (grandchild != NULL) {
            grandchild->parent = _parent;
            grandchild = grandchild->next_sibling;
        }
    }

    tree_item_destroy(_child, _destroy, _arg);

    return _parent;
}

tree_item* tree_item_remove_child(tree_item* _parent, tree_item* _child, tree_item_destroy_custom _destroy, void* _arg)
{
    if (_parent == NULL || _child == NULL) {
        return NULL;
    }

    if (tree_item_include_child(_parent, _child) == false) {
        return NULL;
    }

    return tree_item_remove_selected_child(_parent, _child, _destroy, _arg);
}

tree_item* tree_item_remove_ith(tree_item* _parent, size_t _ith, tree_item_destroy_custom _destroy, void* _arg)
{
    if (_parent == NULL) {
        return NULL;
    }
    tree_item* child = tree_item_get_child(_parent, _ith);
    if (child == NULL) {
        return NULL;
    }

    return tree_item_remove_selected_child(_parent, child, _destroy, _arg);
}

tree_item* tree_item_remove_all_child(tree_item* _parent, tree_item_destroy_custom _destroy, void* _arg)
{
    if (_parent == NULL) {
        return NULL;
    }

    tree_item* child = _parent->head_child;
    while (child != NULL) {
        if (child->head_child != NULL) {
            tree_item_remove_all_child(child, _destroy, _arg);
        }
        tree_item* child_next = child->next_sibling;
        tree_item_destroy(child, _destroy, _arg);
        child = child_next;
    }

    _parent->head_child = NULL;
    _parent->tail_child = NULL;

    return _parent;
}


void* tree_item_get_data(tree_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    void* data = (void*)(_item + 1);

    return data;
}

tree_item* data_get_tree_item(void* _data)
{
    if (_data == NULL) {
        return NULL;
    }

    tree_item* item = (tree_item*)_data - 1;

    return item;

}
