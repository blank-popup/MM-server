#ifndef ___MM_BASE_FILE_H___
#define ___MM_BASE_FILE_H___

#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>


#ifdef __cplusplus
extern "C" {
#endif

    bool file_check_exist(const char* _filepath);
    FILE* file_open_read(const char* _filepath);
    FILE* file_open_write(const char* _filepath);
    FILE* file_open_append(const char* _filepath);
    int file_set_position_begin(FILE* _file, long _offset);
    int file_set_position_end(FILE* _file, long _offset);
    int file_set_position_current(FILE* _file, long _offset);
    long file_get_position(FILE* _file);
    long file_get_size(FILE* _file);
    bool directory_check_exist(const char* _dirpath);
    bool directory_make(const char* _dirpath, mode_t _mode);
    bool file_compare_extension(const char* _filepath, const char* _extension);
    bool directory_make_recursively(char* _filepath, size_t _size, mode_t _mode);
    char* path_get_filename(char* _filepath);
    char* path_get_extension(char* _filepath);

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_FILE_H___
