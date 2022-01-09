#include "mmConfigureExt.h"


int main(int _argc, char* _argv[])
{
    size_t size_mma = sizeof(mma_rr);
    size_t size_mmp = sizeof(mmp_rr);

    fprintf(stderr, "Size of mma_rr: [%lu]\n", size_mma);
    fprintf(stderr, "Size of mm_rr: [%lu]\n", size_mmp);
    fprintf(stderr, "Size of msgsize: [%lu]\n", size_mma > size_mmp ? size_mma : size_mmp);

    return 0;
}
