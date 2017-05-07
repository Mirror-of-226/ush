#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)

#include "../include/builtin_cmd.h"

void umkdir::run(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: umkdir [destination path]");
        return;
    }

    int status;
    status = mkdir(argv[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == -1) {
        printf("umkdir error");
    }
}


#endif