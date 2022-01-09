#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <dlfcn.h>
#include "mmFSM.h"
#include "../../mmLogSimple.h"


struct _FSM {
    JSON_value* root_value;
    JSON_object* root_object;
    size_t count_state;
    JSON_object_item* item_state_entry;
    JSON_object_item* item_state_current;
    char rcode[FSM_SIZE_NAME];
    fsm_action_post action_post;
};


static double convert_pointer_to_double(void* _pointer)
{
    unsigned long addr = (unsigned long)_pointer;

    return (double)addr;
}

static void* convert_double_to_pointer(double _double)
{
    unsigned long addr = (unsigned long)_double;

    return (void*)addr;
}


void* fsm_malloc(size_t _size)
{
    void* memory = malloc(_size);
    memset(memory, 0x00, _size);

    return memory;
}

char* fsm_memset_print(char* _buffer, size_t _size, const char* _message, ...)
{
    if (_buffer == NULL || _message == NULL) {
        return NULL;
    }

    memset(_buffer, 0x00, _size);
    va_list argp;
    va_start(argp, _message);
    vsnprintf(_buffer, _size, _message, argp);
    va_end(argp);

    return _buffer;
}


JSON_value* fsm_fsm_get_root_value(FSM* _fsm)
{
    if (_fsm == NULL) {
        return NULL;
    }

    return _fsm->root_value;
}

void fsm_fsm_set_root_value(FSM* _fsm, JSON_value* _value)
{
    if (_fsm == NULL || _value == NULL) {
        return;
    }

    _fsm->root_value = _value;
    _fsm->root_object = json_value_get_object(_fsm->root_value);
    _fsm->count_state = json_object_get_length(_fsm->root_object);
}

JSON_object* fsm_fsm_get_root_object(FSM* _fsm)
{
    if (_fsm == NULL) {
        return NULL;
    }

    return _fsm->root_object;
}

size_t fsm_fsm_get_count_state(FSM* _fsm)
{
    if (_fsm == NULL) {
        return 0;
    }

    return _fsm->count_state;
}

JSON_object_item* fsm_fsm_get_item_state_entry(FSM* _fsm)
{
    if (_fsm == NULL) {
        return NULL;
    }

    return _fsm->item_state_entry;
}

void fsm_fsm_set_item_state_entry(FSM* _fsm, JSON_object_item* _item)
{
    if (_fsm == NULL || _item == NULL) {
        return;
    }

    JSON_object* object = _fsm->root_object;
    JSON_object_item* item = json_object_get_item_head(object);
    while (item != NULL) {
        if (item == _item) {
            _fsm->item_state_entry = _item;
            return;
        }
        item = json_object_item_get_next(item);
    }
}

JSON_object_item* fsm_fsm_get_item_state_current(FSM* _fsm)
{
    if (_fsm == NULL) {
        return NULL;
    }

    return _fsm->item_state_current;
}

void fsm_fsm_set_item_state_current(FSM* _fsm, JSON_object_item* _item)
{
    if (_fsm == NULL || _item == NULL) {
        return;
    }

    JSON_object* object = _fsm->root_object;
    JSON_object_item* item = json_object_get_item_head(object);
    while (item != NULL) {
        if (item == _item) {
            _fsm->item_state_current = _item;
            return;
        }
        item = json_object_item_get_next(item);
    }
}

char* fsm_fsm_get_rcode(FSM* _fsm)
{
    if (_fsm == NULL) {
        return NULL;
    }

    return _fsm->rcode;
}

void fsm_fsm_set_rcode(FSM* _fsm, char* _rcode)
{
    if (_fsm == NULL || _rcode == NULL) {
        return;
    }

    fsm_memset_print(_fsm->rcode, sizeof(_fsm->rcode), "%s", _rcode);
}

fsm_action_post fsm_fsm_get_action_post(FSM* _fsm)
{
    if (_fsm == NULL) {
        return NULL;
    }

    return _fsm->action_post;
}

void fsm_fsm_set_action_post(FSM* _fsm, fsm_action_post _action_post)
{
    if (_fsm == NULL) {
        return;
    }

    _fsm->action_post = _action_post;
}


void* fsm_get_action(JSON_array* _array, const char* _directory_root)
{
    if (_array == NULL || _directory_root == NULL) {
        return NULL;
    }

    const char* so = json_array_get_string(_array, 0);
    const char* name = json_array_get_string(_array, 1);
    char path_so[FSM_SIZE_PATH] = { 0, };
    fsm_memset_print(path_so, sizeof(path_so), "%s/%s", _directory_root, so);

    void* handle = dlopen(path_so, RTLD_LAZY);
    if (handle == NULL) {
        return NULL;
    }

    void* action = dlsym(handle, name);
    return action;
}

JSON_value* fsm_parse_fsm_root_value(const char* _filepath_fsm, const char* _directory_root)
{
    char filepath[FSM_SIZE_PATH] = { 0, };
    fsm_memset_print(filepath, sizeof(filepath), "%s/%s", _directory_root, _filepath_fsm);
    JSON_value* fsm_root_value = json_parse_file(filepath);
    JSON_object* fsm_root_object = json_value_get_object(fsm_root_value);

    size_t count_state = json_object_get_length(fsm_root_object);
    for (size_t ii = 0; ii < count_state; ++ii) {
        const char* name_state = json_object_get_name(fsm_root_object, ii);
        JSON_object* object_state = json_object_get_object(fsm_root_object, name_state);
        size_t count_event = json_object_get_length(object_state);
        for (size_t jj = 0; jj < count_event; ++jj) {
            const char* name_event = json_object_get_name(object_state, jj);
            JSON_object* object_event = json_object_get_object(object_state, name_event);
            JSON_array* array_action = json_object_get_array(object_event, FSM_NAME_ACTION);

            fsm_action action = (fsm_action)fsm_get_action(array_action, _directory_root);
            if (action != NULL) {
                double pointer = convert_pointer_to_double((void*)action);
                json_object_set_number(object_event, FSM_NAME_POINTER, pointer);
            }
            else {
                json_object_set_string(object_event, FSM_NAME_STRANGE, FSM_PHRASE_STRANGE);
            }
        }
    }

    return fsm_root_value;
}

FSM* fsm_create(JSON_value* _fsm_root_value, fsm_action_post _post)
{
    if (_fsm_root_value == NULL) {
        return NULL;
    }

    FSM* fsm = (FSM*)memloc(sizeof(FSM));
    if (fsm == NULL) {
        return NULL;
    }
    fsm->root_value = _fsm_root_value;
    fsm->root_object = json_value_get_object(fsm->root_value);
    fsm->count_state = json_object_get_length(fsm->root_object);
    fsm->item_state_entry = json_object_get_item_head(fsm->root_object);
    fsm->item_state_current = fsm->item_state_entry;
    memset(fsm->rcode, 0x00, sizeof(fsm->rcode));
    fsm->action_post = _post;

    return fsm;
}

void fsm_destory(FSM* _fsm)
{
    if (_fsm != NULL) {
        json_value_free(_fsm->root_value);
        fsm_free(_fsm);
    }
}

JSON_object_item* fsm_find_item_state(FSM* _fsm, const char* _state)
{
    if (_fsm == NULL || _state == NULL) {
        return NULL;
    }

    JSON_object_item* item = _fsm->item_state_current;
    if (item == NULL) {
        return NULL;
    }

    for (size_t ii = 0; ii < _fsm->count_state; ++ii) {
        const char* name_state = json_object_item_get_name(item);
        if (name_state == NULL) {
            return NULL;
        }
        if (strcmp(_state, name_state) == 0) {
            return item;
        }
        item = json_object_item_get_next(item);
        if (item == NULL) {
            JSON_value* value = json_object_item_get_value(_fsm->item_state_current);
            JSON_value* value_parent = json_value_get_parent(value);
            JSON_object* object_parent = json_value_get_object(value_parent);
            item = json_object_get_item_head(object_parent);
        }
    }

    return NULL;
}

FSM_event fsm_make_event(const char* _name, void* _maker, FSM* _fsm, void* _param0, void* _param1, void* _param2, void* _param3, void* _param4)
{
    FSM_event evt;
    memset(&evt, 0x00, sizeof(FSM_event));
    if (_name != NULL) {
        sprintf(evt.name, "%s", _name);
    }
    evt.maker= _maker;
    evt.fsm = _fsm;
    evt.param0 = _param0;
    evt.param1 = _param1;
    evt.param2 = _param2;
    evt.param3 = _param3;
    evt.param4 = _param4;

    return evt;
}

FSM_event* fsm_event_create(const char* _name, void* _maker, FSM* _fsm, void* _param0, void* _param1, void* _param2, void* _param3, void* _param4)
{
    if (_name == NULL || _fsm == NULL) {
        return NULL;
    }

    FSM_event* evt = (FSM_event*)memloc(sizeof(FSM_event));
    if (evt == NULL) {
        return NULL;
    }
    if (_name != NULL) {
        sprintf(evt->name, "%s", _name);
    }
    evt->maker= _maker;
    evt->fsm = _fsm;
    evt->param0 = _param0;
    evt->param1 = _param1;
    evt->param2 = _param2;
    evt->param3 = _param3;
    evt->param4 = _param4;

    return evt;
}

void fsm_event_destroy(FSM_event* _evt)
{
    if (_evt != NULL) {
        fsm_free(_evt);
    }
}

fsm_rcode fsm_execute_one_state(FSM_event* _evt)
{
    if (_evt == NULL || _evt->fsm == NULL) {
        return NULL;
    }

    JSON_value* value_state = json_object_item_get_value(_evt->fsm->item_state_current);
    JSON_object* object_state = json_value_get_object(value_state);
    JSON_object* object_event = json_object_get_object(object_state, _evt->name);
    if (json_object_has_name(object_event, FSM_NAME_POINTER) == false) {
        return NULL;
    }
    double pointer = json_object_get_real_number(object_event, FSM_NAME_POINTER);
    fsm_action action = (fsm_action)convert_double_to_pointer(pointer);
    fsm_rcode rcode = action(_evt);

    fsm_action_post action_post = _evt->fsm->action_post;
    if (action_post != NULL) {
        rcode = action_post(rcode);
    }

    fsm_memset_print(_evt->fsm->rcode, sizeof(_evt->fsm->rcode), "%s", rcode);

    return rcode;
}

fsm_rcode fsm_execute_nothing()
{
    return FSM_RC_ACK;
}

static bool fsm_state_has_event_default(JSON_object_item* _item_state)
{
    if (_item_state == NULL) {
        return false;
    }

    JSON_value* value_state = json_object_item_get_value(_item_state);
    JSON_object* object_state = json_value_get_object(value_state);

    return json_object_has_name(object_state, FSM_EVENT_DEFAULT);
}

fsm_rcode fsm_execute_event_throughout(FSM_event* _evt)
{
    if (_evt == NULL || _evt->fsm == NULL) {
        return NULL;
    }
    while (true) {
        fsm_rcode rcode = fsm_execute_one_state(_evt);
        if (rcode == NULL) {
            return NULL;
        }
        JSON_object_item* item_state_next = fsm_get_item_state_next(_evt, _evt->fsm);
        if (fsm_state_has_event_default(item_state_next) == false) {
            fsm_memset_print(_evt->fsm->rcode, sizeof(_evt->fsm->rcode), "%s", rcode);
            return rcode;
        }
        fsm_fsm_set_item_state_current(_evt->fsm, item_state_next);
        if (strcmp(FSM_EVENT_DEFAULT, _evt->name) != 0) {
            memset(_evt->name, 0x00, sizeof(_evt->name));
            sprintf(_evt->name, "%s", FSM_EVENT_DEFAULT);
        }
    }

    return NULL;
}

JSON_object_item* fsm_get_item_state_next(FSM_event* _evt, FSM* _fsm)
{
    if (_fsm == NULL || _fsm->rcode == NULL) {
        return NULL;
    }

    JSON_value* value_state = json_object_item_get_value(_fsm->item_state_current);
    JSON_object* object_state = json_value_get_object(value_state);
    JSON_object* object_event = json_object_get_object(object_state, _evt->name);
    JSON_object* object_next = json_object_get_object(object_event, FSM_NAME_NEXT);

    const char* state_next = json_object_get_string(object_next, _fsm->rcode);
    if (state_next == NULL) {
        return NULL;
    }

    JSON_object_item* item_state_next = fsm_find_item_state(_fsm, state_next);

    return item_state_next;
}
