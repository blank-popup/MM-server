#ifndef ___MM_BASE_IPC_H___
#define ___MM_BASE_IPC_H___

#include <mqueue.h>
#include <semaphore.h>


#ifdef __cplusplus
extern "C" {
#endif

    mqd_t mqueue_open(const char* _name, int _oflag, mode_t _mode, __syscall_slong_t _maxmsg, __syscall_slong_t _msgsize);
    int mqueue_send(mqd_t _fd, void* _message, size_t _msgsize, unsigned int _priority);
    ssize_t mqueue_receive(mqd_t _fd, void* _message, size_t _msgsize, unsigned int* _priority);
    int mqueue_close(mqd_t _fd);
    int mqueue_unlink(const char* _name);

    int shmemory_open(const char* _name, int _oflag, mode_t _mode);
    int shmemory_close(int _fd);
    int semaphore_initialize(sem_t* _fd, int _pshared, unsigned int _value);
    sem_t* semaphore_open(const char* _name, int _oflag, mode_t _mode, unsigned int _value);
    int semaphore_wait(sem_t* _fd);
    int semaphore_post(sem_t* _fd);
    int semaphore_get_value(int* _value, sem_t* _fd);
    void* shmemory_mmap(void* _pointer, size_t _length, int _prot, int _flags, int _fd, off_t _offset);
    int shmemory_munmap(sem_t* _fd, size_t _length);
    int semaphore_close(sem_t* _fd);
    int semaphore_unlink(const char* _name);
    int shmemory_unlink(const char* _name);

    int shmemory_write_offset(sem_t* _fd, void* _shmemory, size_t _offset, void* _data, size_t _size_data, size_t _size_type);
    int shmemory_read_offset(sem_t* _fd, void* _data, size_t _size_data, void* _shmemory, size_t _offset);
    int shmemory_write_all(sem_t* _fd, void* _shmemory, void* _data, size_t _size_data);
    int shmemory_read_all(sem_t* _fd, void* _data, size_t _size_data, void* _shmemory);

    int named_pipe_server_open(const char* _path, mode_t _mode, int _oflag);
    int named_pipe_client_open(const char* _path, int _oflag);

#ifdef __cplusplus
}
#endif

#endif // ___MM_BASE_IPC_H___
