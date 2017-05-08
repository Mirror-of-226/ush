#include "../include/builtin_cmd.h"
#include "../include/ush.h"

#ifdef _WIN32


upwd::upwd(char *_now_path) {
	now_path = _now_path;
}

void upwd::run(int argc, char* args[])
{
	printf("%s\n",now_path);
	return;
}


#endif // !WIN32