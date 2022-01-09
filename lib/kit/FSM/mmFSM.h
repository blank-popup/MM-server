#ifndef ___MM_KIT_FSM_H___
#define ___MM_KIT_FSM_H___

#include "kit/json/mmJson.h"
// #include "../../base/mmMemory.h"


#ifdef __cplusplus
extern "C" {
#endif

#define FSM_NAME_ACTION "action"
#define FSM_NAME_OPTION "option"
#define FSM_NAME_NEXT "next"
#define FSM_NAME_POINTER "pointer"

#define FSM_EVENT_DEFAULT ""

#define FSM_RC_ACK "ACK"
#define FSM_RC_NAK "NAK"
#define FSM_RC_DEFAULT FSM_RC_NAK

#define FSM_EXTENSION_SO ".so"
#define FSM_EXTENSION_PYTHON ".py"

#define FSM_NAME_STRANGE "STRANGE"
#define FSM_PHRASE_STRANGE "HERE IS SOMETHING STRANGE"

#define FSM_SIZE_NAME (64)
#define FSM_SIZE_PATH (1024)

    typedef const char* fsm_rcode;
    typedef fsm_rcode(*fsm_action_post)(fsm_rcode);

    typedef struct _FSM FSM;

    typedef struct _FSM_event {
        char name[FSM_SIZE_NAME];
        void* maker;
        FSM* fsm;
        void* param0;
        void* param1;
        void* param2;
        void* param3;
        void* param4;
    } FSM_event;

    typedef fsm_rcode(*fsm_action)(FSM_event*);

    void* fsm_malloc(size_t _size);
#define fsm_free(m) do { free(m); m = NULL; } while(0);
    char* fsm_memset_print(char* _buffer, size_t _size, const char* _message, ...);

#ifndef memloc
#define memloc(s) json_malloc(s)
#endif
#ifndef memfree
#define memfree(m) json_free(m)
#endif

    JSON_value* fsm_fsm_get_root_value(FSM* _fsm);
    void fsm_fsm_set_root_value(FSM* _fsm, JSON_value* _value);
    JSON_object* fsm_fsm_get_root_object(FSM* _fsm);
    size_t fsm_fsm_get_count_state(FSM* _fsm);
    JSON_object_item* fsm_fsm_get_item_state_entry(FSM* _fsm);
    void fsm_fsm_set_item_state_entry(FSM* _fsm, JSON_object_item* _item);
    JSON_object_item* fsm_fsm_get_item_state_current(FSM* _fsm);
    void fsm_fsm_set_item_state_current(FSM* _fsm, JSON_object_item* _item);
    char* fsm_fsm_get_rcode(FSM* _fsm);
    void fsm_fsm_set_rcode(FSM* _fsm, char* _rcode);
    fsm_action_post fsm_fsm_get_action_post(FSM* _fsm);
    void fsm_fsm_set_action_post(FSM* _fsm, fsm_action_post _action_post);

    void* fsm_get_action(JSON_array* _array, const char* _directory_root);
    JSON_value* fsm_parse_fsm_root_value(const char* _filepath_fsm, const char* _directory_root);
    FSM* fsm_create(JSON_value* _fsm_root_value, fsm_action_post _post);
    void fsm_destory(FSM* _fsm);
    JSON_object_item* fsm_find_item_state(FSM* _fsm, const char* _state);
    FSM_event fsm_make_event(const char* _name, void* _maker, FSM* _fsm, void* _param0, void* _param1, void* _param2, void* _param3, void* _param4);
    FSM_event* fsm_event_create(const char* _name, void* _maker, FSM* _fsm, void* _param0, void* _param1, void* _param2, void* _param3, void* _param4);
    void fsm_event_destroy(FSM_event* _evt);
    fsm_rcode fsm_execute_one_state(FSM_event* _evt);
    fsm_rcode fsm_execute_nothing();
    fsm_rcode fsm_execute_event_throughout(FSM_event* _evt);
    JSON_object_item* fsm_get_item_state_next(FSM_event* _evt, FSM* _fsm);


#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_FSM_H___