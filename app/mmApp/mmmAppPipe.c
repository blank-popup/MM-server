#include "mmmAppExt.h"


rcode initialize_app_named_pipe_server()
{
    const c8* path = get_mmApp_self_named_pipe_path();
    if (STRING_IS_NOT_EMPTY(path)) {
        c8 filepath[MM_SIZE_PATH] = { 0, };
        const c8* directory_root = get_mm_directory_root();
        string_memset_print(filepath, sizeof(filepath), "%s/%s", directory_root, path);
        directory_make_recursively(filepath, sizeof(filepath), 0775);

        theApp.sds.np.number= theApp.self.number;
        if ((theApp.sds.np.fd = named_pipe_server_open(filepath, 0666, O_RDWR)) < 0) {
            lerror(l_app, "Fail to open named pipe server[%s]", filepath);
            return mma(nak);
        }
        linfo(l_app, "Initialized named pipe server path[%s] fd[%d]", path, theApp.sds.np.fd);
    }

    return mma(ack);
}
