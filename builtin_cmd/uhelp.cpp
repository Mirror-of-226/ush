#include "../include/builtin_cmd.h"
#include<map>
using namespace std;

uhelp::uhelp(map<std::string, builtin_cmd*>& _mbc) {
    mbc = &_mbc;
}

void uhelp::run(int argc, char **argv) {
    printf("USH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    map<std::string, builtin_cmd*>::iterator it;

    it = mbc->begin();

    while(it != mbc->end())
    {
        printf("  %s\n", it->first.c_str());
        it++;
    }

    printf("Use the man command for information on other programs.\n");
}