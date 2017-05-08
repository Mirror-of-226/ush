#include "../include/builtin_cmd.h"
#include "../include/ush.h"

#ifdef LINUX

#include <unistd.h>

void ucd::run(int argc, char *argv[])
{
    if (argv[1] == NULL) {
      fprintf(stderr, "ush: expected argument to \"cd\"\n");
    } else {
      if (chdir(args[1]) != 0) {
        perror("ush");
      }
    }
}

#endif
#ifdef _WIN32

#include<windows.h>
#include<vector>
using namespace std;

ucd::ucd(char *_now_path) {
	now_path = _now_path;
}

void ucd::run(int argc, char* args[])
{
	WIN32_FIND_DATA findData;
	HANDLE hfind = INVALID_HANDLE_VALUE;
	char temp[BUF_SIZE], temp_local[BUF_SIZE];

	if (!strcmp(args[1], "..")) {
		int len = strlen(now_path);
		if (now_path[len - 1] != ':') {
			int i = len - 1;
			while (i > 1 && now_path[i] != '\\') i--;
			now_path[i] = '\0';
			if (now_path[i - 1] == ':') {
				now_path[i] = '\\';
				now_path[i + 1] = '\0';
			}
		}
		return;
	}

	strcpy(temp, args[1]);
	strcpy(temp_local, now_path);
	int tll = strlen(temp_local);
	if(temp_local[tll-1] != '\\') strcat(temp_local, "\\");
	strcat(temp_local, args[1]);
	
	hfind = FindFirstFile(temp_local, &findData);
	if (hfind == INVALID_HANDLE_VALUE) {
		hfind = FindFirstFile(temp, &findData);
		if (hfind == INVALID_HANDLE_VALUE) {
			printf("path not exist.\n");
		}
		else {
			strcpy(now_path, temp);
		}
	}
	else {
		strcpy(now_path, temp_local);
	}
	return;

}



#endif // _WIN32

