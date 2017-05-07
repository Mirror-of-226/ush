#include "../include/builtin_cmd.h"

#ifdef _WIN32

#include<windows.h>
#include<tlhelp32.h>
#include<stdio.h>

void ukill ::run(int argc, char *argv[]) {
	int pid = 0;
	sscanf(argv[1],"%d",&pid);
	printf("killing process %d\n",pid);
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, pid);
	if (!hProcess) {
		printf("open process failed.\n");
		return;
	}
	TerminateProcess(hProcess, 9);
	printf("process killed.\n");
}

#endif