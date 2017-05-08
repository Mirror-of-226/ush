#include "../include/builtin_cmd.h"
#include "../include/ush.h"

urm::urm(char *_now_path) {
	now_path = _now_path;
}

#ifdef _WIN32

#include "windows.h"


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

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)

#include <iostream>

int rmdir(std::string dir_full_path)
{
	DIR* dirp = opendir(dir_full_path.c_str());
	if (!dirp)
	{
		return -1;
	}
	struct dirent *dir;
	struct stat st;
	while ((dir = readdir(dirp)) != NULL)
	{
		if (strcmp(dir->d_name, ".") == 0
			|| strcmp(dir->d_name, "..") == 0)
		{
			continue;
		}
		std::string sub_path = dir_full_path + '/' + dir->d_name;
		if (lstat(sub_path.c_str(), &st) == -1)
		{
			continue;
		}
		if (S_ISDIR(st.st_mode))
		{
			if (rmdir(sub_path) == -1)
			{
				closedir(dirp);
				return -1;
			}
			rmdir(sub_path.c_str());
		}
		else if (S_ISREG(st.st_mode))
		{
			unlink(sub_path.c_str());
		}
		else
		{
			continue;
		}
	}
	if (rmdir(dir_full_path.c_str()) == -1)
	{
		closedir(dirp);
		return -1;
	}
	closedir(dirp);
	return 0;
}

void urm::run(int argc, char **argv) {
	if (argv[1] == NULL) {
		fprintf(stderr, "ush: ebufpected argument to \"urm\"\n");
	}
	else {
		char path[BUF_SIZE];
		strcpy(path, argv[1]);

		struct stat st;
		if (lstat(path, &st) == -1) {
			perror("ush");
		}
		if (S_ISREG(st.st_mode)) {
			if (unlink(path) == -1) {
				return;
			}
		}
		else if (S_ISDIR(st.st_mode)) {
			if (strcmp(path, ".") == 0 || strcmp(path, ",") == 0) {
				return;
			}
			if (rmdir(path) == -1) {
				return;
			}
		}
	}
}

#endif