#include "../include/builtin_cmd.h"

#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

HANDLE GetDirectoryHandle1(LPCSTR directory)
{
	return CreateFile(directory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
}

void mk_folder(LPCSTR dstfolder)
{
	HANDLE hnddst = GetDirectoryHandle1(dstfolder);
	if (hnddst == INVALID_HANDLE_VALUE)
	{
		BOOL createResult = CreateDirectory(dstfolder, NULL);
		if (createResult == TRUE)
		{
			CloseHandle(hnddst);
			return;
		}
		else
		{
			printf("make directory failed.\n");
			return;
		}
	}
	else
	{
		CloseHandle(hnddst);
		return;
	}

}

void umkdir::run(int argc, char *argv[]) {
	if (argc != 2) {
		printf("usage: umkdir [destination path]");
		return;
	}
	LPSTR dst = argv[1];
	HANDLE hnddst = GetDirectoryHandle1(dst);
	mk_folder(dst);
	CloseHandle(hnddst);
}


#endif