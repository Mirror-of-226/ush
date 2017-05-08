#include "../include/builtin_cmd.h"
#include "../include/ush.h"

#ifdef _WIN32

#include "windows.h"

urm::urm(char *_now_path) {
	now_path = _now_path;
}

void urm::run(int argc, char* args[])
{
	WIN32_FIND_DATA findData;
	HANDLE hfind = INVALID_HANDLE_VALUE;
	char temp[BUF_SIZE], temp_local[BUF_SIZE];

	strcpy(temp, args[1]);
	strcpy(temp_local, now_path);
	int tll = strlen(temp_local);
	if (temp_local[tll - 1] != '\\') strcat(temp_local, "\\");
	strcat(temp_local, args[1]);

	hfind = FindFirstFile(temp_local, &findData);
	if (hfind == INVALID_HANDLE_VALUE) {
		hfind = FindFirstFile(temp, &findData);
		if (hfind == INVALID_HANDLE_VALUE) {
			printf("file not exist.\n");
		}
		else {
			//strcpy(now_path, temp);
			//OFSTRUCT ofs;
			//HFILE hf = OpenFile(temp,&ofs,OF_DELETE);
			if (!DeleteFile(temp)) {
			}
		}
	}
	else {
		//strcpy(now_path, temp_local);
		if (!DeleteFile(temp_local)) {
		}
	}
}


#endif // !WIN32