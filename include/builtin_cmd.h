#ifndef _BUILTIN_FUNC_H
#define _BUILTIN_FUNC_H

class builtin_cmd {
    virtual run(int argc, char *argv[]) = 0;
}
