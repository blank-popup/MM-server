#include <string.h>
#include <stdbool.h>
#include "mmMemory.h"
#include "mmLogSimple.h"


void* memory_allocate(size_t _size)
{
    void* memory = malloc(_size);
    if (memory == NULL) {
        return NULL;
    }
    if (memset(memory, 0x00, _size) != memory) {
        free(memory);
        memory = NULL;
        return NULL;;
    }

    return memory;
}


void* memory_duplicate(void* _source, size_t _size)
{
    if (_source == NULL) {
        return NULL;
    }

    void* memory = memloc(_size);
    if (memory == NULL) {
        return NULL;
    }
    if (memcpy(memory, _source, _size) != memory) {
        if (memory != NULL) {
            memfree(memory);
        }
        return NULL;
    }

    return memory;
}

void* memory_reallocate(void* _source, size_t _capacity_old, size_t _capacity_new)
{
    if (_source == NULL) {
        return NULL;
    }

    void* memory = memloc(_capacity_new);
    if (memory == NULL) {
        return NULL;
    }
    if (memcpy(memory, _source, _capacity_old > _capacity_new ? _capacity_new : _capacity_old) != memory) {
        memfree(memory);
        return NULL;
    }
    memfree(_source);

    return memory;
}

void* memory_double_capacity(void* _source, size_t _capacity)
{
    if (_source == NULL) {
        return NULL;
    }

    return memory_reallocate(_source, _capacity, 2 * _capacity);
}


typedef struct _memory_tracer_item {
    void* memory;
    size_t size;
    char* filepath;
    int line;
    struct _memory_tracer_item* previous;
    struct _memory_tracer_item* next;
} memory_tracer_item;

typedef struct _memory_tracer {
    bool trace;
    size_t count_malloc;
    size_t count_free;
    memory_tracer_item* head;
    memory_tracer_item* tail;
} memory_tracer;

static memory_tracer* ___mm_memory_tracer = NULL;


void* memory_allocate_trace(size_t _size, char* _filepath, int _line)
{
    void* memory = memory_allocate(_size);
    if (memory == NULL) {
        return NULL;
    }

    if (___mm_memory_tracer != NULL && _filepath != NULL) {
        memory_tracer_add_item(memory, _size, _filepath, _line);
    }

    return memory;
}


int memory_tracer_initialize()
{
    if (___mm_memory_tracer == NULL) {
        ___mm_memory_tracer = malloc(sizeof(memory_tracer));
        memset(___mm_memory_tracer, 0x00, sizeof(memory_tracer));
    }

    return 0;
}

int memory_tracer_add_item(void* _memory, size_t _size, char* _filepath, int _line)
{
    if (___mm_memory_tracer == NULL) {
        return -1;
    }

    memory_tracer_item* item = malloc(sizeof(memory_tracer_item));
    memset(item, 0x00, sizeof(memory_tracer_item));
    item->memory = _memory;
    item->size = _size;
    item->filepath = malloc(strlen(_filepath) + 1);
    memset(item->filepath, 0x00, strlen(_filepath) + 1);
    strcpy(item->filepath, _filepath);
    item->line = _line;

    if (___mm_memory_tracer->head == NULL) {
        item->previous = NULL;
        item->next = NULL;
        ___mm_memory_tracer->head = item;
        ___mm_memory_tracer->tail = item;
    }
    else {
        item->previous = ___mm_memory_tracer->tail;
        item->next = NULL;
        ___mm_memory_tracer->tail->next = item;
        ___mm_memory_tracer->tail = item;
    }
    ++___mm_memory_tracer->count_malloc;

    return 0;
}

int memory_tracer_remove_item(void* _memory, char* _filepath, int _line)
{
    if (___mm_memory_tracer == NULL || _filepath == NULL) {
        return -1;
    }

    bool found = false;
    for (memory_tracer_item* item = ___mm_memory_tracer->head; item != NULL; item = item->next) {
        if (item->memory == _memory) {
            found = true;
            ++___mm_memory_tracer->count_free;
            if (___mm_memory_tracer->head->memory == _memory && ___mm_memory_tracer->tail->memory == _memory) {
                ___mm_memory_tracer->head = NULL;
                ___mm_memory_tracer->tail = NULL;
            }
            else if (___mm_memory_tracer->head->memory == _memory) {
                item->next->previous = NULL;
                ___mm_memory_tracer->head = item->next;
            }
            else if (___mm_memory_tracer->tail->memory == _memory) {
                item->previous->next = NULL;
                ___mm_memory_tracer->tail = item->previous;
            }
            else {
                item->previous->next = item->next;
                item->next->previous = item->previous;
            }
            break;
        }
    }

    if (found == false) {
        llerror("Free memory not allocated [%s : %d]", _filepath, _line);
    }

    return 0;
}

int memory_tracer_print(const char* _title, const char* _prefix)
{
    llerror("Title: [%s]", _title);
    if (___mm_memory_tracer == NULL || ___mm_memory_tracer->head == NULL) {
        llerror("There is no memory allocated\n");
        return 0;
    }

    llerror("--- Memory that is not freed ---\n");
    for (memory_tracer_item* item = ___mm_memory_tracer->head; item != NULL; item = item->next) {
        llerror("[[%s]] pointer:[%p] size: [%lu] (%s, %d)\n", _prefix, item->memory, item->size, item->filepath, item->line);
    }

    return 0;
}

int memory_tracer_destroy()
{
    if (___mm_memory_tracer == NULL) {
        return -1;
    }
    memory_tracer_item* item = ___mm_memory_tracer->head;
    while (item != NULL) {
        memory_tracer_item* tracer_next = item->next;
        free(item->filepath);
        free(item);
        item = tracer_next;
    }

    free(___mm_memory_tracer);
    ___mm_memory_tracer = NULL;

    return 0;
}
