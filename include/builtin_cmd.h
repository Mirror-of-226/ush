#ifndef _BUILTIN_FUNC_H
#define _BUILTIN_FUNC_H

class builtin_cmd {
    virtual run(const char* args[]) = 0;
}