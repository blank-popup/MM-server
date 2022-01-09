#ifndef ___MM_KIT_STRUCTURE_QUEUE_H___
#define ___MM_KIT_STRUCTURE_QUEUE_H___

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct _queue_item queue_item;
    typedef struct _queue queue;

    typedef bool(*queue_item_destroy_custom)(queue_item*, void*);

    queue* queue_malloc(size_t _size);
#define queue_free(m) do { free(m); m = NULL; } while(0);

#ifndef memloc
#define memloc(s) queue_malloc(s)
#endif
#ifndef memfree
#define memfree(m) queue_free(m)
#endif

    queue* queue_create();
    queue_item* queue_item_create(void* _data, size_t _size_data);
    bool queue_item_destroy(queue_item* _item, queue_item_destroy_custom _destroy, void* _arg);
    bool queue_destroy(queue* _q, queue_item_destroy_custom _destroy, void* _arg);

    queue* queue_put_item(queue* _q, queue_item* _item);
    queue_item* queue_get_item(queue* _q);
    size_t queue_get_count(queue* _q);
    void* queue_item_get_data(queue_item* _item);
    queue_item* data_get_queue_item(void* _data);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_STRUCTURE_QUEUE_H___
