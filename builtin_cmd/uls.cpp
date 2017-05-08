#include "../include/builtin_cmd.h"
#include "../include/ush.h"

uls::uls(char *_now_path) {
	now_path = _now_path;
}

#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

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

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
#include "../include/ush.h"

void uls::run(int argc, char *argv[])
{
	char dir[BUF_SIZE];
	if (argv[1] == NULL) {
		getcwd(dir, sizeof(dir));
	}
	else {
		strcpy(dir, argv[1]);
	}
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	struct passwd *tf;
	struct group *gf;

	char buf[512];

	if ((dp = opendir(dir)) == NULL) {
		perror("ush");
		return;
	}

	while ((entry = readdir(dp)) != NULL) {
		sprintf(buf, "%s/%s", dir, entry->d_name);
		stat(buf, &statbuf);

		switch (statbuf.st_mode & S_IFMT) {
		case S_IFBLK: printf("b "); break;
		case S_IFCHR: printf("c "); break;
		case S_IFDIR: printf("d "); break;
		case S_IFIFO: printf("p "); break;
		case S_IFLNK: printf("l "); break;
		case S_IFSOCK:printf("s "); break;
		default:printf("- "); break;
		}

		printf((statbuf.st_mode & S_IRUSR) ? " r" : " -");
		printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
		printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
		printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
		printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
		printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
		printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
		printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
		printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

		printf("\t%d ", statbuf.st_nlink);

		tf = getpwuid(statbuf.st_uid);
		printf("\t%s ", tf->pw_name);

		gf = getgrgid(statbuf.st_gid);
		printf("\t%s ", gf->gr_name);

		printf("%lld", statbuf.st_size);
		printf(" %s", entry->d_name);
		printf(" %s", ctime(&statbuf.st_mtime));
	}
	closedir(dp);
}

#endif