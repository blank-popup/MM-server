#include <string.h>
#include "mmLinkedList.h"
#include "../../mmLogSimple.h"


struct _linked_list_item {
    struct _linked_list_item* next;
    struct _linked_list_item* previous;
};

struct _linked_list {
    struct _linked_list_item* head;
    struct _linked_list_item* tail;
};


void* linked_list_malloc(size_t _size)
{
    linked_list* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}


linked_list_item* linked_list_item_get_next(linked_list_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->next;
}

linked_list_item* linked_list_item_get_previous(linked_list_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    return _item->previous;
}

linked_list_item* linked_list_get_head(linked_list* _ll)
{
    if (_ll == NULL) {
        return NULL;
    }

    return _ll->head;
}

linked_list_item* linked_list_get_tail(linked_list* _ll)
{
    if (_ll == NULL) {
        return NULL;
    }

    return _ll->tail;
}

linked_list* linked_list_create()
{
    linked_list* ll = (linked_list*)memloc(sizeof(linked_list));
    return ll;
}

linked_list_item* linked_list_item_create(void* _data, size_t _size_data)
{
    if (_size_data == 0) {
        return NULL;
    }

    linked_list_item* item = (linked_list_item*)memloc(sizeof(linked_list_item) + _size_data);
    if (_data != NULL) {
        memcpy(item + 1, _data, _size_data);
    }
    return item;
}

bool linked_list_item_destroy(linked_list_item* _item, linked_list_item_destroy_custom _destroy, void* _arg)
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

bool linked_list_destroy(linked_list* _ll, linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_ll == NULL) {
        return false;
    }

    linked_list_item* item = _ll->head;
    while (item != NULL) {
        linked_list_item* item_next = item->next;
        if (linked_list_item_destroy(item, _destroy, _arg) == false) {
            return false;
        };
        item = item_next;
    }

    memfree(_ll);

    return true;
}


linked_list* linked_list_append_item(linked_list* _ll, linked_list_item* _item)
{
    if (_ll == NULL || _item == NULL) {
        return NULL;
    }

    if (_ll->head == NULL) {
        _ll->head = _item;
        _ll->tail = _item;
    }
    else {
        _ll->tail->next = _item;
        _item->previous = _ll->tail;
        _ll->tail = _item;
    }

    return _ll;
}

linked_list* linked_list_insert_item(linked_list* _ll, linked_list_item* _item, size_t _ith)
{
    if (_ll == NULL || _item == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_ith(_ll, _ith);
    if (item == NULL) {
        return NULL;
    }
    linked_list_item* item_previous = item->previous;
    linked_list_item* item_next = item;

    if (item_previous == NULL) {
        _ll->head = _item;
    }
    else {
        item_previous->next = _item;
        _item->previous = item_previous;
    }
    if (item_next == NULL) {
        _ll->tail = _item;
    }
    else {
        item_next->previous = _item;
        _item->next = item_next;
    }

    return _ll;
}

size_t linked_list_get_count(linked_list* _ll)
{
    if (_ll == NULL) {
        return 0;
    }

    size_t count = 0;
    linked_list_item* item = _ll->head;
    while (item != NULL) {
        ++count;
        item = item->next;
    }

    return count;
}

linked_list_item* linked_list_get_ith(linked_list* _ll, size_t _ith)
{
    if (_ll == NULL) {
        return NULL;
    }

    linked_list_item* item = _ll->head;
    for (size_t ii = 0; ii < _ith; ++ii) {
        if (item == NULL) {
            return NULL;
        }
        item = item->next;
    }

    return item;
}

bool linked_list_include_item(linked_list* _ll, linked_list_item* _item)
{
    if (_ll == NULL || _ll->head == NULL || _item == NULL) {
        return false;
    }

    linked_list_item* item = _ll->head;
    while (item != NULL) {
        if (item == _item) {
            return true;
        }
        item = item->next;
    }

    return false;
}

static linked_list* linked_list_disconnect_item(linked_list* _ll, linked_list_item* _item)
{
    if (_ll == NULL || _item == NULL || linked_list_include_item(_ll, _item) == false) {
        return NULL;
    }

    linked_list_item* item_previous = _item->previous;
    linked_list_item* item_next = _item->next;

    if (item_previous == NULL) {
        _ll->head = _ll->head->next;
    }
    else {
        item_previous->next = item_next;
    }
    if (item_next == NULL) {
        _ll->tail = _ll->tail->previous;
    }
    else {
        item_next->previous = item_previous;
    }

    return _ll;
}

static linked_list* linked_list_disconnect_ith(linked_list* _ll, size_t _ith)
{
    if (_ll == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_ith(_ll, _ith);
    if (item == NULL) {
        return NULL;
    }
    linked_list_disconnect_item(_ll, item);

    return _ll;
}

linked_list_item* linked_list_pop_ith(linked_list* _ll, size_t _ith)
{
    if (_ll == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_ith(_ll, _ith);
    if (item == NULL) {
        return NULL;
    }
    linked_list_disconnect_item(_ll, item);

    return item;
}

linked_list* linked_list_remove_item(linked_list* _ll, linked_list_item* _item, linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_ll == NULL || _item == NULL || linked_list_include_item(_ll, _item) == false) {
        return NULL;
    }

    linked_list_disconnect_item(_ll, _item);
    linked_list_item_destroy(_item, _destroy, _arg);

    return _ll;
}

linked_list* linked_list_remove_ith(linked_list* _ll, size_t _ith, linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_ll == NULL) {
        return NULL;
    }

    linked_list_item* item = linked_list_get_ith(_ll, _ith);
    if (item == NULL) {
        return NULL;
    }

    linked_list_disconnect_item(_ll, item);
    linked_list_item_destroy(item, _destroy, _arg);

    return _ll;
}

linked_list* linked_list_remove_all_item(linked_list* _ll, linked_list_item_destroy_custom _destroy, void* _arg)
{
    if (_ll == NULL || _ll->head == NULL) {
        return NULL;
    }

    linked_list_item* item = _ll->head;
    while (item != NULL) {
        linked_list_item* item_next = item->next;
        linked_list_item_destroy(item, _destroy, _arg);
        item = item_next;
    }

    _ll->head = NULL;
    _ll->tail = NULL;

    return _ll;
}


void* linked_list_item_get_data(linked_list_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    void* data = (void*)(_item + 1);

    return data;
}

linked_list_item* data_get_linked_list_item(void* _data)
{
    if (_data == NULL) {
        return NULL;
    }

    linked_list_item* item = (linked_list_item*)_data - 1;

    return item;
}
