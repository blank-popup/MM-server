#include <string.h>
#include "mmCircularLinkedList.h"
#include "../../mmLogSimple.h"


struct _circular_linked_list_item {
    struct _circular_linked_list_item* next;
    struct _circular_linked_list_item* previous;
};


circular_linked_list_item* circular_linked_list_malloc(size_t _size)
{
    circular_linked_list_item* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}


circular_linked_list_item* circular_linked_list_item_get_next(circular_linked_list_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->next;
}

circular_linked_list_item* circular_linked_list_item_get_previous(circular_linked_list_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->previous;
}


circular_linked_list_item* circular_linked_list_item_create(void* _data, size_t _size_data)
{
    if (_size_data == 0) {
        return NULL;
    }

    circular_linked_list_item* item = (circular_linked_list_item*)memloc(sizeof(circular_linked_list_item) + _size_data);
    memset(item, 0x00, sizeof(circular_linked_list_item) + _size_data);
    item->next = item;
    item->previous = item;
    if (_data != NULL) {
        memcpy(item + 1, _data, _size_data);
    }
    return item;
}

bool circular_linked_list_item_destroy(circular_linked_list_item* _item, circular_linked_list_item_destroy_custom _destroy, void* _arg)
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


circular_linked_list_item* circular_linked_list_item_append_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target)
{
    if (_item == NULL || _item_target == NULL) {
        return NULL;
    }

    _item_target->next = _item;
    _item_target->previous = _item->next;
    _item->previous->next = _item_target;
    _item->previous = _item_target;

    return _item;
}

circular_linked_list_item* circular_linked_list_item_insert_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target, size_t _ith)
{
    if (_item == NULL || _item_target == NULL) {
        return NULL;
    }

    circular_linked_list_item* item = circular_linked_list_item_get_item(_item, _ith);
    if (item == NULL) {
        return NULL;
    }
    circular_linked_list_item* item_previous = item->previous;
    circular_linked_list_item* item_next = item;

    _item_target->next = item_next;
    _item_target->previous = item_previous;
    item_previous->next = _item_target;
    item_next->previous = _item_target;

    return _item;
}

size_t circular_linked_list_item_get_count(circular_linked_list_item* _item)
{
    if (_item == NULL) {
        return 0;
    }

    size_t count = 0;
    circular_linked_list_item* item = _item;
    while (item != NULL) {
        ++count;
        item = item->next;
        if (item == _item) {
            break;
        }
    }

    return count;
}

circular_linked_list_item* circular_linked_list_item_get_item(circular_linked_list_item* _item, size_t _ith)
{
    if (_item == NULL) {
        return NULL;
    }

    circular_linked_list_item* item = _item;
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (item == NULL) {
            return NULL;
        }
        item = item->next;
    }

    return item;
}

bool circular_linked_list_item_include_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target)
{
    if (_item == NULL || _item_target == NULL) {
        return false;
    }

    circular_linked_list_item* item = _item;
    while (item != NULL) {
        if (item == _item_target) {
            return true;
        }
        item = item->next;
        if (item == _item) {
            break;
        }
    }

    return false;
}

static bool circular_linked_list_item_remove_selected_item(circular_linked_list_item* _item, circular_linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_item == NULL) {
        return false;
    }

    circular_linked_list_item* item_previous = _item->previous;
    circular_linked_list_item* item_next = _item->next;

    item_previous->next = item_next;
    item_next->previous = item_previous;

    circular_linked_list_item_destroy(_item, _destroy, _arg);

    return true;
}

circular_linked_list_item* circular_linked_list_item_remove_item(circular_linked_list_item* _item, circular_linked_list_item* _item_target, circular_linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_item == NULL || _item_target == NULL) {
        return NULL;
    }
    if (circular_linked_list_item_include_item(_item, _item_target) == false) {
        return NULL;
    }

    if (circular_linked_list_item_remove_selected_item(_item_target, _destroy, _arg) == true) {
        return _item;
    }
    else {
        return NULL;
    }
}

circular_linked_list_item* circular_linked_list_item_remove_ith(circular_linked_list_item* _item, size_t _ith, circular_linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_item == NULL) {
        return NULL;
    }

    circular_linked_list_item* item = circular_linked_list_item_get_item(_item, _ith);
    if (item == NULL) {
        return NULL;
    }

    if (circular_linked_list_item_remove_selected_item(item, _destroy, _arg) == true) {
        return _item;
    }
    else {
        return NULL;
    }
}

bool circular_linked_list_item_remove_all_item(circular_linked_list_item* _item, circular_linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_item == NULL) {
        return NULL;
    }

    circular_linked_list_item* item = _item;
    while (item != NULL) {
        circular_linked_list_item* item_next = item->next;
        circular_linked_list_item_destroy(item, _destroy, _arg);
        item = item_next;
        if (item == _item) {
            break;
        }
    }

    return true;
}


void* circular_linked_list_item_get_data(circular_linked_list_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    void* data = (void*)(_item + 1);

    return data;
}

circular_linked_list_item* data_get_circular_linked_list_item(void* _data)
{
    if (_data == NULL) {
        return NULL;
    }

    circular_linked_list_item* item = (circular_linked_list_item*)_data - 1;

    return item;
}
