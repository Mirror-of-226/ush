#include "../include/builtin_cmd.h"
#include "../include/ush.h"

upwd::upwd(char *_now_path) {
	now_path = _now_path;
}

#ifdef _WIN32

void upwd::run(int argc, char* args[])
{
	printf("%s\n",now_path);
	return;
}


#endif // !WIN32

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)

void upwd::run(int argc, char *argv[]) {

	char buf[BUF_SIZE];
	getcwd(buf, sizeof(buf));
	printf("%s\n", buf);
}

#endif