#include <string.h>
#include "mmStack.h"
#include "../../mmLogSimple.h"


struct _stack_item {
    struct _stack_item* next;
    struct _stack_item* previous;
};

struct _stack {
    struct _stack_item* head;
    struct _stack_item* tail;
};


stack* stack_malloc(size_t _size)
{
    stack* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}


stack* stack_create()
{
    stack* s = (stack*)memloc(sizeof(stack));
    return s;
}

stack_item* stack_item_create(void* _data, size_t _size_data)
{
    if (_size_data == 0) {
        return NULL;
    }

    stack_item* item = (stack_item*)memloc(sizeof(stack_item) + _size_data);
    if (_data != NULL) {
        memcpy(item + 1, _data, _size_data);
    }
    return item;
}

bool stack_item_destroy(stack_item* _item, stack_item_destroy_custom _destroy, void* _arg)
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

bool stack_destroy(stack* _s, stack_item_destroy_custom _destroy, void* _arg)
{
    if (_s == NULL) {
        return false;
    }

    stack_item* item = _s->head;
    while (item != NULL) {
        stack_item* item_next = item->next;
        if (stack_item_destroy(item, _destroy, _arg) == false) {
            return false;
        };
        item = item_next;
    }

    memfree(_s);

    return true;
}


stack* stack_push_item(stack* _s, stack_item* _item)
{
    if (_s == NULL || _item == NULL) {
        return NULL;
    }

    if (_s->head == NULL) {
        _s->head = _item;
        _s->tail = _item;
    }
    else {
        _s->tail->next = _item;
        _item->previous = _s->tail;
        _s->tail = _item;
    }

    return _s;
}

stack_item* stack_pop_item(stack* _s)
{
    if (_s == NULL || _s->head == NULL) {
        return NULL;
    }

    stack_item* item = _s->tail;
    if (_s->head == _s->tail) {
        _s->head = NULL;
        _s->tail = NULL;
    }
    else {
        _s->tail = _s->tail->previous;
        _s->tail->next = NULL;
    }

    return item;
}

size_t stack_get_count(stack* _s)
{
    if (_s == NULL) {
        return 0;
    }

    size_t count = 0;
    stack_item* item = _s->head;
    while (item != NULL) {
        ++count;
        item = item->next;
    }

    return count;
}

void* stack_item_get_data(stack_item* _item)
{
    if (_item == NULL) {
        return NULL;
    }

    void* data = (void*)(_item + 1);

    return data;
}

stack_item* data_get_stack_item(void* _data)
{
    if (_data == NULL) {
        return NULL;
    }

    stack_item* item = (stack_item*)_data - 1;

    return item;
}
