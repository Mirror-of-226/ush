#include "../include/builtin_cmd.h"

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)

#include "../include/builtin_cmd.h"
#include "../include/ush.h"

void setTime(char *path, struct stat st);
void copyFile(char *src, char *dst);
void walkDir(char *src, char *dst);
void copyDir(char *src, char *dst);
void copySymLink(char *src, char *dst);

void setTime(char *path, struct stat st)
{
	struct utimbuf timebuf;
	timebuf.actime = st.st_atime;
	timebuf.modtime = st.st_mtime;
	if ((utime(path, &timebuf)) == -1) {
		printf("utime error.\n");
	}
}

void copyFile(char *src, char *dst)
{
	struct stat st;
	lstat(src, &st);
	if (dst[strlen(dst) - 1] == '/') {
		strcat(dst, src);
	}
	int srcFile = open(src, O_RDONLY);
	int dstFile = open(dst, O_RDONLY);
	if (dstFile >= 0) {
		printf("ucp: %s: File already exits\n", dst);
		printf("Do you want to replace the existing file (yes/no)? ");
		char input[BUF_SIZE];
		while (1) {
			scanf("%s", input);
			if (strcmp("yes", input) == 0) {
				break;
			}
			else if (strcmp("no", input) == 0) {
				close(srcFile);
				close(dstFile);
				exit(1);
			}
			else {
				printf("Please type 'yes' or 'no': ");
			}
		}
	}

	umask(0);
	unsigned int mode = (0777) & st.st_mode;
	dstFile = creat(dst, mode);

	int n;
	char buf[BUF_SIZE];
	while ((n = read(srcFile, buf, BUF_SIZE)) > 0) {
		if (write(dstFile, buf, n) != n) {
			printf("write error\n");
			close(srcFile);
			close(dstFile);
			exit(-1);
		}
	}
	setTime(dst, st);

	close(srcFile);
	close(dstFile);
}

void walkDir(char *src, char *dst)
{
	struct dirent *entry;
	DIR* dir;
	if ((dir = opendir(src)) == NULL) {
		printf("opendir error\n");
		exit(1);
	}
	char srcpath[BUF_SIZE], dstpath[BUF_SIZE];

	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

		strcpy(srcpath, src);
		strcat(srcpath, "/");
		strcat(srcpath, entry->d_name);
		strcpy(dstpath, dst);
		strcat(dstpath, "/");
		strcat(dstpath, entry->d_name);

		switch (entry->d_type) {
		case DT_REG: copyFile(srcpath, dstpath); break;
		case DT_DIR: copyDir(srcpath, dstpath); break;
		case DT_LNK: copySymLink(srcpath, dstpath); break;
		default: break;
		}
		//printf("%s %s\n", srcpath, dstpath);
	}
	closedir(dir);
}

void copyDir(char *src, char *dst)
{
	struct stat st;
	lstat(src, &st);
	DIR *srcDir = opendir(src);
	DIR *dstDir = opendir(dst);

	if (dstDir != NULL) {
		printf("mycp: %s: Directory already exits\n", dst);
		printf("Do you want to replace the existing Directory (yes/no)? ");
		char input[BUF_SIZE];
		while (1) {
			scanf("%s", input);
			if (strcmp("yes", input) == 0) {
				break;
			}
			else if (strcmp("no", input) == 0) {
				closedir(srcDir);
				closedir(dstDir);
				exit(1);
			}
			else {
				printf("Please type 'yes' or 'no': ");
			}
		}
	}

	umask(0);
	unsigned int mode = (0777) & st.st_mode;
	mkdir(dst, mode);
	walkDir(src, dst);
	setTime(dst, st);

}

void copySymLink(char *src, char *dst)
{
	struct stat st;
	lstat(src, &st);

	char buf[BUF_SIZE];
	int srcLink = readlink(src, buf, BUF_SIZE);
	if (srcLink <= 0) {
		printf("readlink error\n");
		exit(1);
	}
	buf[srcLink] = 0;
	if (symlink(buf, dst) == -1) {
		printf("symlink error\n");
	}
	umask(0);
	unsigned int mode = (0777) & st.st_mode;
	chmod(dst, mode);

#if defined(__linux__) || defined(linux) || defined(__linux)
	struct timespec times[2] = { st.st_atim, st.st_mtim };
	int result = utimensat(AT_FDCWD, dst, times, AT_SYMLINK_NOFOLLOW);
#endif

}

void ucp::run(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage:\n  ucp [src] [des]\n");
		exit(1);
	}
	char src[BUF_SIZE], dst[BUF_SIZE];
	strcpy(src, argv[1]);
	strcpy(dst, argv[2]);
	struct stat st;
	if (lstat(src, &st) == 0) {
		switch (st.st_mode & S_IFMT) {
		case S_IFREG: copyFile(src, dst); break;
		case S_IFDIR: copyDir(src, dst); break;
		case S_IFLNK: copySymLink(src, dst); break;
		default: break;
		}
	}
	else {
		printf("ucp: %s: No such file or directory\n", src);
	}
}

#endif



#ifdef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define BUFFERSIZE 4096

#include <vector>
#include <string>
using namespace std;
vector<char*> errors;

typedef enum COPY_RESULT
{
	COPY_RESULT_SUCCESS,
	COPY_RESULT_SRC_CANNOT_OPEN,
	COPY_RESULT_DST_EXIST,
	COPY_RESULT_ERROR
} COPY_RESULT;

HANDLE GetFileHandle(LPCSTR file)
{
	return CreateFile(file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT, NULL);
}
HANDLE GetDirectoryHandle(LPCSTR directory)
{
	return CreateFile(directory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
}

// 设置文件夹属性
BOOL set_folder_attr(LPCSTR srcfolder, LPCSTR dstfolder)
{
	HANDLE hndsrc = GetDirectoryHandle(srcfolder);
	if (hndsrc == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	HANDLE hnddst = CreateFile(dstfolder, GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hnddst != INVALID_HANDLE_VALUE)
	{
		FILE_BASIC_INFO basicInfo;
		GetFileInformationByHandleEx(hndsrc, FileBasicInfo, &basicInfo, sizeof(basicInfo));
		if (SetFileInformationByHandle(hnddst, FileBasicInfo, &basicInfo, sizeof(basicInfo)) == FALSE)
		{
			int err = GetLastError();
			printf("%d\n", err);
		}
		CloseHandle(hndsrc);
		CloseHandle(hnddst);
		return TRUE;
	}
	else
	{
		CloseHandle(hndsrc);
		CloseHandle(hnddst);
		return FALSE;
	}
}
// 创建文件夹, 并且不复制属性
COPY_RESULT copy_folder(LPCSTR srcfolder, LPCSTR dstfolder)
{
	HANDLE hndsrc = GetDirectoryHandle(srcfolder);
	if (hndsrc == INVALID_HANDLE_VALUE)
	{
		return COPY_RESULT_SRC_CANNOT_OPEN;
	}
	HANDLE hnddst = GetDirectoryHandle(dstfolder);
	if (hnddst == INVALID_HANDLE_VALUE)
	{
		BOOL createResult = CreateDirectoryEx(srcfolder, dstfolder, NULL);
		if (createResult == TRUE)
		{
			CloseHandle(hndsrc);
			CloseHandle(hnddst);
			return COPY_RESULT_SUCCESS;
		}
		else
		{
			CloseHandle(hndsrc);
			return COPY_RESULT_DST_EXIST;
		}
	}
	else
	{
		CloseHandle(hndsrc);
		CloseHandle(hnddst);
		return COPY_RESULT_DST_EXIST;
	}

}

// 复制文件, 并且复制属性
COPY_RESULT copy_file(LPCSTR srcfile, LPCSTR dstfile, LPCSTR filename)
{
	HANDLE hndsrc = CreateFile(srcfile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hndsrc == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hndsrc);
		//errors.push_back("Source File Cannot Open For Read: " + srcfile);
		return COPY_RESULT_SRC_CANNOT_OPEN;
	}
	else
	{
		HANDLE hnddst = CreateFile(dstfile, GENERIC_WRITE | FILE_WRITE_ATTRIBUTES, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hnddst == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hndsrc);
			CloseHandle(hnddst);
			return COPY_RESULT_DST_EXIST;
		}
		else
		{
			// 写文件内容
			BYTE buffer[BUFFERSIZE];
			DWORD bytesread;
			DWORD byteswritten;
			BOOL result_read = TRUE;
			BOOL result_write = TRUE;
			DWORD err;
			while (1)
			{
				result_read = ReadFile(hndsrc, buffer, BUFFERSIZE, &bytesread, NULL);
				if (result_read == FALSE)
				{
					err = GetLastError();
					//throw
					break;
				}
				else if (bytesread == 0)
				{
					break;
				}
				else
				{
					result_write = WriteFile(hnddst, buffer, bytesread, &byteswritten, NULL);
					if (result_write == FALSE)
					{
						err = GetLastError();
						//throw
						break;
					}
				}
			}
			if (result_write == TRUE)
			{
				FlushFileBuffers(hnddst);

				// 复制文件属性
				FILE_BASIC_INFO basicInfo;
				result_read = GetFileInformationByHandleEx(hndsrc, FileBasicInfo, &basicInfo, sizeof(basicInfo));
				result_read = SetFileInformationByHandle(hnddst, FileBasicInfo, &basicInfo, sizeof(basicInfo));
			}
			else
			{
				//throw
			}
			CloseHandle(hndsrc);
			CloseHandle(hnddst);
			return COPY_RESULT_SUCCESS;
		}
	}


}

void enum_files(LPSTR srcFolderName, LPSTR dstFolderName)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA findData;

	LPSTR tPath = (char*)malloc(sizeof(char) * 260);
	strcpy(tPath, srcFolderName);
	strcat(tPath, "\\*");
	hFind = FindFirstFile(tPath, &findData);

	if (hFind == INVALID_HANDLE_VALUE)
	{

	}
	else
	{
		// 用于存储当前处理对象的完整路径
		char tSrcPath[260];
		strcpy(tSrcPath, srcFolderName);
		strcat(tSrcPath, "\\");
		DWORD szsrcpath = strlen(tSrcPath);


		// 存储复制时新对象的完整路径
		char tDstPath[260];
		strcpy(tDstPath, dstFolderName);
		strcat(tDstPath, "\\");
		DWORD szdstpath = strlen(tDstPath);

		HANDLE hndFile;
		HANDLE hndFolder;

		COPY_RESULT result;
		do
		{
			if (findData.cFileName[0] == '.' && findData.cFileName[1] == 0)
			{
			}
			else if (findData.cFileName[0] == '.' && findData.cFileName[1] == '.' && findData.cFileName[2] == 0)
			{
			}

			else
			{
				tSrcPath[szsrcpath] = 0;
				strcat(tSrcPath, findData.cFileName);
				tDstPath[szdstpath] = 0;
				strcat(tDstPath, findData.cFileName);

				// 处理文件夹
				if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FALSE)
				{
					hndFolder = GetDirectoryHandle(tSrcPath);
					// 符号链接
					if ((findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != FALSE)
					{

					}
					// 非符号链接
					else
					{
						result = copy_folder(tSrcPath, tDstPath);
						if (result == COPY_RESULT_SUCCESS || COPY_RESULT_DST_EXIST)
						{
							enum_files(tSrcPath, tDstPath);
							if (set_folder_attr(tSrcPath, tDstPath) == FALSE)
								printf("err?\n");
						}
						else
						{
							printf("err d \n\tsrc:%s\n\tdst:%s\n", tSrcPath, tDstPath);
							//throw
						}
					}
					CloseHandle(hndFolder);
				}
				// 处理文件
				else
				{
					hndFile = GetFileHandle(tSrcPath);
					// 符号链接
					if ((findData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != FALSE)
					{

					}
					// 非符号链接
					else
					{
						result = copy_file(tSrcPath, tDstPath, findData.cFileName);
						if (result != COPY_RESULT_SUCCESS && result != COPY_RESULT_DST_EXIST)
						{
							printf("err f \n\tsrc:%s\n\tdst:%s\n", tSrcPath, tDstPath);
							//throw
						}
					}
					CloseHandle(hndFile);
				}
			}
		} while (FindNextFile(hFind, &findData) == TRUE);

	}
	FindClose(hFind);
}

void ucp::run(int argc, char* args[])
{
	if (argc != 3) {
		printf("usage: ucp [source path] [destination path]");
		return;
	}
	errors.clear();
	LPSTR src = args[1], dst = args[2];
	//STR2TSTR(args[1], &src);
	//STR2TSTR(args[2], &dst);
	HANDLE hndsrc = GetDirectoryHandle(src), hnddst = GetDirectoryHandle(dst);

	if (hndsrc == INVALID_HANDLE_VALUE)
		printf("cannot open folder %s\n", args[1]);
	else
	{
		if (hnddst == INVALID_HANDLE_VALUE)
		{
			int err = GetLastError();
			if (err == ERROR_FILE_NOT_FOUND)
			{
				copy_folder(src, dst);
			}
		}
		enum_files(src, dst);
	}
	CloseHandle(hndsrc);
	CloseHandle(hnddst);
	return;
}




#endif // !WIN32
