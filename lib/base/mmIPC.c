#include <sys/socket.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "mmIPC.h"


mqd_t mqueue_open(const char* _name, int _oflag, mode_t _mode, __syscall_slong_t _maxmsg, __syscall_slong_t _msgsize)
{
    if (_name == NULL) {
        return -1;
    }

    struct mq_attr attr;
    attr.mq_maxmsg = _maxmsg;
    attr.mq_msgsize = _msgsize;

    return mq_open(_name, _oflag, _mode, &attr);
}

int mqueue_send(mqd_t _fd, void* _message, size_t _msgsize, unsigned int _priority)
{
    if (_message == NULL) {
        return -1;
    }
    return mq_send(_fd, (const char*)_message, _msgsize, _priority);
}

ssize_t mqueue_receive(mqd_t _fd, void* _message, size_t _msgsize, unsigned int* _priority)
{
    if (_message == NULL) {
        return -1;
    }

    return mq_receive(_fd, _message, _msgsize, _priority);
}

int mqueue_close(mqd_t _fd)
{
    return mq_close(_fd);
}

int mqueue_unlink(const char* _name)
{
    if (_name == NULL) {
        return -1;
    }

    return mq_unlink(_name);
}


int shmemory_open(const char* _name, int _oflag, mode_t _mode)
{
    if (_name == NULL) {
        return -1;
    }

    return shm_open(_name, _oflag, _mode);
}

int shmemory_close(int _fd)
{
    return close(_fd);
}

int semaphore_initialize(sem_t* _fd, int _pshared, unsigned int _value)
{
    if (_fd == NULL) {
        return -1;
    }

    return sem_init(_fd, _pshared, _value);
}

sem_t* semaphore_open(const char* _name, int _oflag, mode_t _mode, unsigned int _value)
{
    if (_name == NULL) {
        return NULL;
    }

    return sem_open(_name, _oflag, _mode, _value);
}

int semaphore_wait(sem_t* _fd)
{
    if (_fd == NULL) {
        return -1;
    }

    return sem_wait(_fd);
}

int semaphore_post(sem_t* _fd)
{
    if (_fd == NULL) {
        return -1;
    }

    return sem_post(_fd);
}

int semaphore_get_value(int* _value, sem_t* _fd)
{
    if (_value == NULL || _fd == NULL) {
        return -1;
    }

    return sem_getvalue(_fd, _value);
}

void* shmemory_mmap(void* _pointer, size_t _length, int _prot, int _flags, int _fd, off_t _offset)
{
    if (_pointer == NULL) {
        return NULL;
    }

    return mmap(_pointer, _length, _prot, _flags, _fd, _offset);
}

int shmemory_munmap(sem_t* _fd, size_t _length)
{
    if (_fd == NULL) {
        return -1;
    }

    return munmap(_fd, _length);
}

int semaphore_close(sem_t* _fd)
{
    if (_fd == NULL) {
        return -1;
    }

    return sem_close(_fd);
}

int semaphore_unlink(const char* _name)
{
    if (_name == NULL) {
        return -1;
    }

    return sem_unlink(_name);
}

int shmemory_unlink(const char* _name)
{
    if (_name == NULL) {
        return -1;
    }

    return shm_unlink(_name);
}

int shmemory_write_offset(sem_t* _fd, void* _shmemory, size_t _offset, void* _data, size_t _size_data, size_t _size_type)
{
    if (_fd == NULL || _shmemory == NULL || _data == NULL) {
        return -1;
    }

    if (semaphore_wait(_fd) < 0) {
        return -1;
    }
    memcpy((void*)((char*)_shmemory + _offset), _data, _size_data);
    memset((void*)((char*)_shmemory + _offset + _size_data), 0x00, _size_type - _size_data);
    if (semaphore_post(_fd) < 0) {
        return -1;
    }

    return 0;
}

int shmemory_read_offset(sem_t* _fd, void* _data, size_t _size_data, void* _shmemory, size_t _offset)
{
    if (_fd == NULL || _data == NULL || _shmemory == NULL) {
        return -1;
    }

    if (semaphore_wait(_fd) < 0) {
        return -1;
    }
    memcpy(_data, (void*)((char*)_shmemory + _offset), _size_data);
    if (semaphore_post(_fd) < 0) {
        return -1;
    }

    return 0;
}

int shmemory_write_all(sem_t* _fd, void* _shmemory, void* _data, size_t _size_data)
{
    return shmemory_write_offset(_fd, _shmemory, 0, _data, _size_data, _size_data);
}

int shmemory_read_all(sem_t* _fd, void* _data, size_t _size_data, void* _shmemory)
{
    return shmemory_read_offset(_fd, _data, _size_data, _shmemory, 0);
}


int named_pipe_server_open(const char* _path, mode_t _mode, int _oflag)
{
    if (_path == NULL) {
        return -1;
    }

    mkfifo(_path, _mode);

    return open(_path, _oflag);
}

int named_pipe_client_open(const char* _path, int _oflag)
{
    if (_path == NULL) {
        return -1;
    }

    return open(_path, _oflag);
}
