#include "../include/builtin_cmd.h"
#include "../include/ush.h"

#include<map>
using namespace std;

utype::utype(map<std::string, builtin_cmd*>& _mbc) {
	mbc = &_mbc;
}

void utype::run(int argc, char *argv[]) {
	if ((*mbc).find(argv[1]) != (*mbc).end()) {
		printf("%s is a shell builtin.\n", argv[1]);
	}
	else printf("%s not found.\n", argv[1]);
}

