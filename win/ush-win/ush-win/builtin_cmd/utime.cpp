#include "../include/builtin_cmd.h"

#ifdef _WIN32

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>

void utime::run(int argc, char *argv[]) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if (argc == 1)
	{
		printf("Usage: %s [cmdline]\n", argv[0]);
		return;
	}
	TCHAR cmd[2000];
	int ed = 0;
	for (int i = 1; i < argc; i++) {
		int len = strlen(argv[i]);
		strcpy(cmd + ed, argv[i]);
		cmd[ed + len] = ' ';
		ed += len + 1;
	}
	cmd[ed] = '\0';
	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		cmd,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}

	SYSTEMTIME startTime, endTime;
	GetSystemTime(&startTime);

	// Wait until child process exits.
	switch (WaitForSingleObject(pi.hProcess, INFINITE)) {
	case WAIT_FAILED:
		printf("The function has failed (%d).\n", GetLastError());
		break;
	case WAIT_TIMEOUT:
		printf("The time-out interval elapsed, and the object's state is nonsignaled.\n");
		break;
	case WAIT_ABANDONED:
		printf("The specified object is a mutex object that was not released by the thread that owned the mutex object before the owning thread terminated. Ownership of the mutex object is granted to the calling thread and the mutex state is set to nonsignaled.");
		break;
	case WAIT_OBJECT_0:
		GetSystemTime(&endTime);
		ULARGE_INTEGER ft_st, ft_ed;
		SystemTimeToFileTime(&startTime, (PFILETIME)&ft_st);
		SystemTimeToFileTime(&endTime, (PFILETIME)&ft_ed);
		ULONGLONG res = ft_ed.QuadPart - ft_st.QuadPart;
		int us, ms, sec, min, h;
		res /= 10;
		us = res % 1000;
		res /= 1000;
		ms = res % 1000;
		res /= 1000;
		sec = res % 60;
		res /= 60;
		min = res % 60;
		res /= 60;
		h = res;
		printf("child process exit. \n%dh %dm %dsec %dms %dus total.\n", h, min, sec, ms, us);
		break;

	}
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

}

#elif LINUX

#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/wait.h>

void utime::run(int argc, char *args[]) {
	pid_t fpid;
	if (argc == 1) {
		printf("usage: mytime command\n");
		return;
	}
	fpid = fork();
	if (fpid < 0) printf("error in fork");
	else if (!fpid) execvp(args[1], args + 1);
	else {
		timeval st, ed;
		int res = gettimeofday(&st, NULL);
		int status = 0;
		pid_t pt = wait(&status);
		if (pt > 0) {
			res = gettimeofday(&ed, NULL);
			int usec = ed.tv_usec - st.tv_usec;
			int sec = ed.tv_sec - st.tv_sec;
			if (usec < 0) {
				usec += 1000000;
				sec--;
			}
			int temp = sec;
			sec = temp % 60;
			temp /= 60;
			int min = temp % 60;
			temp /= 60;
			int hour = temp;
			printf("child process exit. \n%dh %dm %ds %dms total\n", hour, min, sec, usec / 1000);
		}
		else {
			printf("err in child process.\n");
		}
	}
}

#endif // WINDOWS
