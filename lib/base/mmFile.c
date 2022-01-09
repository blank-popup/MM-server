#include <dirent.h>
#include <string.h>
#include "mmFile.h"
#include "base/mmString.h"


bool file_check_exist(const char* _filepath)
{
    if (_filepath == NULL) {
        return false;
    }

    FILE* fp = NULL;
    if ((fp = fopen(_filepath, "r")) == NULL) {
        return false;
    }

    fclose(fp);
    return true;
}

FILE* file_open_read(const char* _filepath)
{
    if (_filepath == NULL) {
        return NULL;
    }

    return fopen(_filepath, "r");
}

FILE* file_open_write(const char* _filepath)
{
    if (_filepath == NULL) {
        return NULL;
    }

    return fopen(_filepath, "w");
}

FILE* file_open_append(const char* _filepath)
{
    if (_filepath == NULL) {
        return NULL;
    }

    return fopen(_filepath, "a");
}

int file_set_position_begin(FILE* _file, long _offset)
{
    if (_file == NULL) {
        return -1;
    }

    return fseek(_file, _offset, SEEK_SET);
}

int file_set_position_end(FILE* _file, long _offset)
{
    if (_file == NULL) {
        return -1;
    }

    return fseek(_file, _offset, SEEK_END);
}

int file_set_position_current(FILE* _file, long _offset)
{
    if (_file == NULL) {
        return -1;
    }

    return fseek(_file, _offset, SEEK_CUR);
}

long file_get_position(FILE* _file)
{
    if (_file == NULL) {
        return -1;
    }

    return ftell(_file);
}

long file_get_size(FILE* _file)
{
    if (_file == NULL) {
        return -1;
    }

    if (file_set_position_end(_file, 0) < 0) {
        return -1;
    }
    return file_get_position(_file);
}

bool directory_check_exist(const char* _dirpath)
{
    if (_dirpath == NULL) {
        return false;
    }

    DIR* dir = NULL;
    if ((dir = opendir(_dirpath)) == NULL) {
        return false;
    }

    closedir(dir);
    return true;
}

bool directory_make(const char* _dirpath, mode_t _mode)
{
    if (_dirpath == NULL) {
        return false;
    }
    if (directory_check_exist(_dirpath) == true) {
        return true;
    }

    // mode: 0775
    if (mkdir(_dirpath, _mode) < 0) {
        return false;
    }

    return true;
}

bool file_compare_extension(const char* _filepath, const char* _extension)
{
    if (_filepath == NULL || _extension == NULL) {
        return false;
    }

    char* ptr = string_search_reverse(_filepath, ".");
    if (ptr == NULL) {
        return false;
    }

    if (strcmp(ptr, _extension) == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool directory_make_recursively(char* _filepath, size_t _size, mode_t _mode)
{
    if (_filepath == NULL) {
        return false;
    }

    char dirpath[_size];
    memset(dirpath, 0x00, _size);
    snprintf(dirpath, _size, "%s", _filepath);
    for(char* p = dirpath + 1; *p; p++) {
        if(*p == '/') {
            *p = 0x00;
            directory_make(dirpath, _mode);
            *p = '/';
        }
    }

    return true;
}

char* path_get_filename(char* _filepath)
{
    if (_filepath == NULL) {
        return NULL;
    }

    char* str = strrchr(_filepath, '/');

    return str;
}

char* path_get_extension(char* _filepath)
{
    if (_filepath == NULL) {
        return NULL;
    }

    char* str = strrchr(_filepath, '.');

    return str;
}
