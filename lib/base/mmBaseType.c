#include "mmBaseType.h"


rcode areturn(rcode _rc, action_return _ar, const c8* _tag, const c8* _filepath, const c8* _function, i32 _line)
{
    if (_ar != NULL) {
        _ar(_rc, _tag, _filepath, _function, _line);
    }

    return _rc;
}
