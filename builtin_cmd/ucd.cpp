#include "../include/builtin_cmd.h"

void ucd::run(int argc, char *argv[])
{
    if (argv[1] == NULL) {
      fprintf(stderr, "ush: expected argument to \"ucd\"\n");
    } else {
      if (chdir(argv[1]) != 0) {
        perror("ush");
      }
    }
}
