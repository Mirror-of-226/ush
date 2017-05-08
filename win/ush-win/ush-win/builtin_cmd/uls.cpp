#include "../include/builtin_cmd.h"
#include "../include/ush.h"

#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

uls::uls(char *_now_path) {
	now_path = _now_path;
}

void uls::run(int argc, char* args[])
{	
	WIN32_FIND_DATA findData;
	HANDLE hfind = INVALID_HANDLE_VALUE;
	char temp[BUF_SIZE];
	if (argc == 1) {
		strcpy(temp, now_path);
		strcat(temp, "\\*");
	}
	else if (argc == 2) {
		strcpy(temp, args[1]);
		strcat(temp, "\\*");
	}
	else {
		printf("usage: ls [path]\n");
		return;
	}
	hfind = FindFirstFile(temp, &findData);
	if (hfind == INVALID_HANDLE_VALUE) {
		printf("err path not exist.\n");
		return;
	}
	do {
		printf("%s\n",findData.cFileName);
	} while (FindNextFile(hfind, &findData) == TRUE);

	FindClose(hfind);
	return;
}




#endif // !WIN32