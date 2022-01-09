#include "mmmAppExt.h"


// rcode initialize_common_shared_memory_server(shm_app_info* _shm_app, const c8* _name_shm, const c8* _name_sem)
// {
//     if (_shm_app == NULL || _name_shm == NULL || strcmp("", _name_shm) == 0 || _name_sem == NULL || strcmp("", _name_sem) == 0) {
//         return mma(nak);
//     }

//     shmemory_unlink(_name_shm);
//     if ((_shm_app->fd = shmemory_open(_name_shm,  O_RDWR | O_CREAT | O_EXCL, 0666)) < 0) {
//         lerror(l_app, "Fail to open common shared memory name[%s]", _name_shm);
//         return mma(nak);
//     }
//     linfo(l_app, "Open common shared memory name[%s] fd[%d]", _name_shm, _shm_app->fd);
//     size_t size = sizeof(app_info);
//     if (ftruncate(_shm_app->fd, size) < 0) {
//         lerror(l_app, "Fail to ftruncate common shared memory name[%s] fd[%d]", _name_shm, _shm_app->fd);
//         return mma(nak);
//     }
//     _shm_app->app = (app_info*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, _shm_app->fd, 0);
//     linfo(l_app, "Initalized common shared memory name[%s] fd[%d]", _name_shm, _shm_app->fd);
//     semaphore_unlink(_name_sem);
//     if ((_shm_app->sem = semaphore_open(_name_sem, O_CREAT, 0644, 1)) == NULL) {
//         lerror(l_app, "Fail to open common semaphore name[%s]", _name_sem);
//         return mma(nak);
//     }
//     linfo(l_app, "Open common semaphore name[%s]", _name_sem);
//     memset(_shm_app->app, 0xFF, size);

//     return mma(ack);
// }

// rcode initialize_common_shared_memory_client()
// {
//     if (strcmp("", theApp.cds.shm_app.name_shm) == 0 || strcmp("", theApp.cds.shm_app.name_sem) == 0) {
//         return mma(nak);
//     }

//     while (true) {
//         time_sleep_select(0, 100000);
//         if ((theApp.cds.shm_app.fd = shmemory_open(theApp.cds.shm_app.name_shm,  O_RDWR, 0666)) < 0) {
//             ldebug(l_app, "Fail to open common shared memory name[%s]", theApp.cds.shm_app.name_shm);
//             continue;
//         }

//         break;
//     }

//     linfo(l_app, "Open common shared memory name[%s] fd[%d]", theApp.cds.shm_app.name_shm, theApp.cds.shm_app.fd);
//     size_t size = sizeof(app_info);
//     if (ftruncate(theApp.cds.shm_app.fd, size) < 0) {
//         lerror(l_app, "Fail to ftruncate common shared memory name[%s] fd[%d]", theApp.cds.shm_app.name_shm, theApp.cds.shm_app.fd);
//         return mma(nak);
//     }
//     theApp.cds.shm_app.app = (app_info*)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, theApp.cds.shm_app.fd, 0);
//     linfo(l_app, "Initalized common shared memory name[%s] fd[%d]", theApp.cds.shm_app.name_shm, theApp.cds.shm_app.fd);

//     if ((theApp.cds.shm_app.sem = semaphore_open(theApp.cds.shm_app.name_sem, O_CREAT, 0644, 1)) == NULL) {
//         lerror(l_app, "Fail to open common semaphore name[%s]", theApp.cds.shm_app.name_sem);
//         return mma(nak);
//     }
//     linfo(l_app, "Open common semaphore name[%s]", theApp.cds.shm_app.name_sem);

//     return mma(ack);
// }

// rcode set_common_shared_memory(shm_app_info* _shm_app, JSON_value* _config_value, c8* _arg_json)
// {
//     if (_shm_app == NULL || _config_value == NULL || _arg_json == NULL) {
//         return mma(nak);
//     }

//     semaphore_wait(_shm_app->sem);
//     app_info* app = _shm_app->app;
//     JSON_object* object = json_value_get_object(_config_value);
//     JSON_object* object_mm = json_object_get_object(object, NAME_MM);
//     const c8* directory_root = json_object_get_string(object_mm, NAME_DIRECTORY_ROOT);
//     const c8* filename_main = json_object_get_string(object_mm, NAME_FILENAME_MAIN);
//     JSON_object* object_shared_memory = json_object_get_object(object_mm, NAME_SHARED_MEMORY);
//     const c8* name_shm = json_object_get_string(object_shared_memory, NAME_NAME_SHM);
//     const c8* name_sem = json_object_get_string(object_shared_memory, NAME_NAME_SEM);
//     snprintf(app->path_daemon, sizeof(app->path_daemon), "%s/%s", directory_root, filename_main);
//     app->path_daemon[strlen(app->path_daemon)] = '\0';
//     snprintf(app->file_daemon, sizeof(app->file_daemon), "%s", filename_main);
//     app->file_daemon[strlen(app->file_daemon)] = '\0';
//     snprintf(app->config, sizeof(app->config), "%s/%s", directory_root, _arg_json);
//     app->config[strlen(app->config)] = '\0';

//     JSON_object* object_mmApps = json_object_get_object(object, NAME_MM_APPS);
//     size_t length_mmApp = json_object_get_length(object_mmApps);
//     size_t count_process = 0;
//     i32 number_process = 0;
//     for (size_t ii = 0; ii < length_mmApp; ++ii) {
//         const c8* type_process = json_object_get_name(object_mmApps, ii);
//         snprintf(app->type_process[ii], sizeof(app->type_process[ii]), "%s", type_process);
//         app->type_process[ii][strlen(app->type_process[ii])] = '\0';

//         JSON_object* object_mmApp = json_object_get_ith_object(object_mmApps, ii);
//         size_t c_process = (size_t)json_object_get_real_number(object_mmApp, NAME_COUNT_PROCESS);
//         count_process += c_process;
//         for (size_t jj = 0; jj < c_process; ++jj) {
//             process_info* process = &app->processes[number_process];
//             process->ID.type = (i32)ii;
//             process->ID.ith = (i32)jj;
//             process->number = number_process;
//             if (ii == length_mmApp - 1 && jj == c_process - 1) {
//                 process->next = 0;
//             }
//             else {
//                 process->next = number_process + 1;
//             }

//             JSON_array* array_sub_thread = json_object_get_array(object_mmApp, NAME_SUB_THREAD);
//             size_t length_sub_thread = json_array_get_length(array_sub_thread);
//             size_t count_thread = 0;
//             i32 number_thread = 0;
//             for (size_t kk = 0; kk < length_sub_thread; ++ kk) {
//                 JSON_object* object_sub_thread = json_array_get_object(array_sub_thread, kk);
//                 size_t c_thread = (size_t)json_object_get_real_number(object_sub_thread, NAME_COUNT_THREAD);
//                 count_thread += c_thread;
//                 for (size_t ll = 0; ll < c_thread; ++ll) {
//                     thread_info* thread = &process->threads[number_thread];
//                     thread->ID.type = (i32)kk;
//                     thread->ID.ith = (i32)ll;
//                     thread->number = number_thread;
//                     thread->running = false;
//                     ++number_thread;
//                 }
//             }
//             process->count_thread = count_thread;
//             ++number_process;
//         }
//     }
//     _shm_app->count_process = count_process;
//     semaphore_post(_shm_app->sem);

//     return mma(ack);
// }
