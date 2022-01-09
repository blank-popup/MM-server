#include <string.h>
#include "mmQueue.h"
#include "../../mmLogSimple.h"


struct _queue_item {
    struct _queue_item* next;
    struct _queue_item* previous;
};

struct _queue {
    struct _queue_item* head;
    struct _queue_item* tail;
};


queue* queue_malloc(size_t _size)
{
    queue* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}


queue* queue_create()
{
    queue* q = (queue*)memloc(sizeof(queue));
    return q;
}

queue_item* queue_item_create(void* _data, size_t _size_data)
{
    if (_size_data == 0) {
        return NULL;
    }

    queue_item* item = (queue_item*)memloc(sizeof(queue_item) + _size_data);
    if (_data != NULL) {
        memcpy(item + 1, _data, _size_data);
    }
    return item;
}

bool queue_item_destroy(queue_item* _item, queue_item_destroy_custom _destroy, void* _arg)
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

bool queue_destroy(queue* _q, queue_item_destroy_custom _destroy, void* _arg)
{
    if (_q == NULL) {
        return false;
    }

    queue_item* item = _q->head;
    while (item != NULL) {
        queue_item* item_next = item->next;
        if (queue_item_destroy(item, _destroy, _arg) == false) {
            return false;
        };
        item = item_next;
    }

    memfree(_q);

    return true;
}


queue* queue_put_item(queue* _q, queue_item* _item)
{
    if (_q == NULL || _item == NULL) {
        return NULL;
    }

    if (_q->head == NULL) {
        _q->head = _item;
        _q->tail = _item;
    }
    else {
        _q->tail->next = _item;
        _item->previous = _q->tail;
        _q->tail = _item;
    }

    return _q;
}

queue_item* queue_get_item(queue* _q)
{
    if (_q == NULL || _q->head == NULL) {
        return NULL;
    }

    queue_item* item = _q->head;
    _q->head = _q->head->next;

    return item;
}

size_t queue_get_count(queue* _q)
{
    if (_q == NULL) {
        return 0;
    }

    size_t count = 0;
    queue_item* item = _q->head;
    while (item != NULL) {
        ++count;
        item = item->next;
    }

    return count;
}

void* queue_item_get_data(queue_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    void* data = (void*)(_item + 1);

    return data;
}

queue_item* data_get_queue_item(void* _data)
{
    if (_data == NULL) {
        return NULL;
    }

    queue_item* item = (queue_item*)_data - 1;

    return item;
}
