#ifndef ___MM_KIT_BASE_STRING_H___
#define ___MM_KIT_BASE_STRING_H___

#include <stddef.h>
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif

#ifndef ___DEFINED__MSTRING___
#define ___DEFINED__MSTRING___
    typedef struct _mstring {
        char* value;
        size_t length;
    } mstring;
#endif

#define STRING_EMPTY ""
#define STRING_IS_NOT_EMPTY(s) (s != NULL && strcmp(s, STRING_EMPTY) != 0)

    mstring* mstring_malloc(const char* _string, const size_t _length);
#define mstring_free(m) do { if (m->value != NULL) memfree(m->value); memfree(m); m = NULL; } while(0);
    int string_memset_print(char* _buffer, size_t _size, const char* _message, ...);

    int integer_reverse(int _integer);
    char* string_replace_all(const char* _source, const char* _oldvalue, const char* _newvalue);
    char* string_split_get_last_word(const char* _phrase, char _delimiter);
    char* string_search_reverse(const char* _haystack, const char* _needle);
    char* string_stristr(const char* _haystack, const char* _needle);
    char* string_join_char_pointer_array(const char* _phrases[], size_t _length_phrase, const char* _coupler);

    mstring mstring_strchr_hereafter(mstring _haystack, size_t _after, char _needle);
    mstring mstring_strchr(mstring _haystack, char _needle);
    mstring mstring_strstr_hereafter(mstring _haystack, size_t _after, mstring _needle);
    mstring mstring_strstr(mstring _haystack, mstring _needle);
    char* mstring_memset_print(mstring _phrase);

    char* string_upper(const char* _phrase);
    char* string_lower(const char* _phrase);

#ifdef __cplusplus
}
#endif

#endif // ___MM_KIT_BASE_STRING_H___
