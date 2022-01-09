#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "base/mmMemory.h"
#include "mmString.h"
#include "../mmLogSimple.h"


mstring* mstring_malloc(const char* _string, const size_t _length)
{
    if (_string == NULL) {
        return NULL;
    }

    mstring* mstr = memloc(sizeof(mstring));
    if (mstr == NULL) {
        return NULL;
    }
    mstr->value = memloc((_length + 1) * sizeof(char));
    if (mstr->value == NULL) {
        return NULL;
    }
    strncpy(mstr->value, _string, _length);
    mstr->length = _length;

    return mstr;
}

int string_memset_print(char* _buffer, size_t _size, const char* _message, ...)
{
    int l = -1;
    if (_buffer == NULL || _message == NULL) {
        return l;
    }

    memset(_buffer, 0x00, _size);
    va_list argp;
    va_start(argp, _message);
    l = vsnprintf(_buffer, _size, _message, argp);
    va_end(argp);

    return l;
}


int integer_reverse(int _integer)
{
    char* c = (char*)&_integer;
    return *(int*)(char[]) { c[3], c[2], c[1], c[0] };
}

char* string_replace_all(const char* _source, const char* _oldvalue, const char* _newvalue)
{
    if (_source == NULL || _oldvalue == NULL || _newvalue == NULL) {
        return NULL;
    }

    size_t count = 0;
    const char* source = _source;
    while (*source != '\0') {
        char* found = NULL;
        if ((found = strstr(source, _oldvalue)) != NULL ) {
            ++count;
            source = found + 1;
        }
        else {
            break;
        }
    }

    char* result = memloc((strlen(_source) + (strlen(_newvalue) - strlen(_oldvalue)) * count + 1) * sizeof(char));
    size_t ii = 0;
    source = _source;
    while (*source != '\0') {
        if (strncmp(source, _oldvalue, strlen(_oldvalue)) == 0) {
            strcpy(result + ii, _newvalue);
            ii += strlen(_newvalue);
            source += strlen(_oldvalue);
        }
        else {
            *(result + ii++) = *source++;
        }
    }

    *(result + ii) = '\0';

    return result;
}

char* string_split_get_last_word(const char* _phrase, char _delimiter)
{
    if (_phrase == NULL) {
        return NULL;
    }

    char* str = strrchr(_phrase, _delimiter);
    if (str != NULL) {
        ++str;
    }

    return str;
}

char* string_search_reverse(const char* _haystack, const char* _needle)
{
    if (_haystack == NULL || _needle == NULL) {
        return NULL;
    }

    int length_haystack = strlen(_haystack);
    int length_needle = strlen(_needle);
    int position = length_haystack - length_needle;

    if (length_haystack == 0 || length_needle == 0 || position < 0) {
        return NULL;
    }

    while (position >= 0) {
        if (strncmp(_haystack + position, _needle, length_needle) == 0) {
            return (char*)_haystack + position;
        }
        position--;
    }

    return NULL;
}

char* string_stristr(const char* _haystack, const char* _needle)
{
    if (_haystack == NULL || _needle == NULL) {
        return NULL;
    }

    const char* haystack = _haystack;
    const char* needle = _needle;

    while (strlen(haystack) >= strlen(needle)) {
        const char* char_haystack = haystack;
        const char* char_needle = needle;
        while (*char_needle != 0 && *char_haystack != 0) {
            if (tolower((int)*char_haystack) != tolower((int)*char_needle)) {
                break;
            }
            ++char_haystack;
            ++char_needle;
        }

        if (*char_needle == 0) {
            return (char*)haystack;
        }
        ++haystack;
    }

    return NULL;
}

char* string_join_char_pointer_array(const char* _phrases[], size_t _length_phrase, const char* _coupler)
{
    if (_phrases == NULL || _coupler == NULL) {
        return NULL;
    }

    size_t length = 0;
    for (size_t ii = 0; ii < _length_phrase; ++ii) {
        length += strlen(_phrases[ii]);
    }

    char* result = memloc((length + strlen(_coupler) * (_length_phrase - 1) + 1) * sizeof(char));
    for (size_t ii = 0; ii < _length_phrase; ++ii) {
        sprintf(result + strlen(result), "%s", _phrases[ii]);
        if (ii < _length_phrase - 1) {
            sprintf(result + strlen(result), "%s", _coupler);
        }
    }

    return result;
}


mstring mstring_strchr_hereafter(mstring _haystack, size_t _after, char _needle)
{
    if (_haystack.value == NULL) {
        return (mstring){ NULL, 0 };
    }

    mstring haystack = _haystack;
    mstring mstr = { 0 };
    for (size_t ii = _after; ii < haystack.length; ++ii) {
        if (haystack.value[ii] == _needle) {
            mstr.value = haystack.value + ii;
            mstr.length = haystack.length - ii;
            return mstr;
        }
    }

    return (mstring){ NULL, 0 };
}

mstring mstring_strchr(mstring _haystack, char _needle)
{
    return mstring_strchr_hereafter(_haystack, 0, _needle);
}

mstring mstring_strstr_hereafter(mstring _haystack, size_t _after, mstring _needle)
{
    if (_haystack.value == NULL || _needle.value == NULL) {
        return (mstring){ NULL, 0 };
    }

    mstring haystack = (mstring){ _haystack.value + _after, _haystack.length - _after };
    mstring needle = _needle;

    while (haystack.length >= needle.length) {
        char* char_haystack = haystack.value;
        size_t length_haystack = haystack.length;
        char* char_needle = needle.value;
        size_t length_needle = needle.length;
        while (*char_haystack != 0 && *char_needle != 0 && length_haystack > 0 && length_needle > 0) {
            if (*char_haystack != *char_needle) {
                break;
            }
            ++char_haystack;
            ++char_needle;
            --length_haystack;
            --length_needle;
        }

        if (length_needle == 0) {
            mstring mstr = { 0 };
            mstr.value = haystack.value;
            mstr.length = haystack.length - (haystack.value - _haystack.value);
            return mstr;
        }

        haystack = (mstring){ haystack.value + 1, haystack.length - 1};
    }

    return (mstring){ NULL, 0 };
}

mstring mstring_strstr(mstring _haystack, mstring _needle)
{
    return mstring_strstr_hereafter(_haystack, 0, _needle);
}

char* mstring_memset_print(mstring _phrase)
{
    if (_phrase.value == NULL) {
        return NULL;
    }

    char* buffer = memloc((_phrase.length + 1) * sizeof(char));
    memset(buffer, 0x00, _phrase.length + 1);
    snprintf(buffer, _phrase.length + 1, "%s", _phrase.value);

    return buffer;
}

char* string_upper(const char* _phrase)
{
    if (_phrase == NULL) {
        return NULL;
    }

    char* upper = memloc((strlen(_phrase) + 1) * sizeof(char));
    for (size_t ii = 0; ii < strlen(_phrase); ++ii) {
        if (_phrase[ii] >= 'a' && _phrase[ii] <= 'z') {
            upper[ii] = _phrase[ii] - 32;
        }
        else {
            upper[ii] = _phrase[ii];
        }
    }

    return upper;
}

char* string_lower(const char* _phrase)
{
    if (_phrase == NULL) {
        return NULL;
    }

    char* lower = memloc((strlen(_phrase) + 1) * sizeof(char));
    for (size_t ii = 0; ii < strlen(_phrase); ++ii) {
        if (_phrase[ii] >= 'A' && _phrase[ii] <= 'Z') {
            lower[ii] = _phrase[ii] + 32;
        }
        else {
            lower[ii] = _phrase[ii];
        }
    }

    return lower;
}
