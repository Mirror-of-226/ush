#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)

#include "../include/builtin_cmd.h"
#include "../include/ush.h"

void upwd::run(int argc, char *argv[]) {

    char buf[BUF_SIZE];
    getcwd(buf, sizeof(buf));
    printf("%s\n", buf);
}

#endif