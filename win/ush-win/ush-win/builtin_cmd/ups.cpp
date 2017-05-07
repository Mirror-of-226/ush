#include "../include/builtin_cmd.h"

#ifdef _WIN32

#include<windows.h>
#include<tlhelp32.h>
#include<stdio.h>

void ups::run(int argc, char *argv[]) {
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 ProEntry32;
	ProEntry32.dwSize = sizeof(ProEntry32);
	Process32First(hProcessSnap, &ProEntry32);
	printf("path:%-25s pid:%-7d priority:%d\n",ProEntry32.szExeFile, ProEntry32.th32ProcessID, ProEntry32.pcPriClassBase);
	while (Process32Next(hProcessSnap, &ProEntry32)) {
		printf("path:%-25s pid:%-7d priority:%d\n", ProEntry32.szExeFile, ProEntry32.th32ProcessID, ProEntry32.pcPriClassBase);
	}
}

#endif // !_WIN32

