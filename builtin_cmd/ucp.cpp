#include "../include/builtin_cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>

void ucp::run(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n  ucp [src] [des]\n");
        exit(1);
    }
    char src[NAMESIZE], dst[NAMESIZE];
    strcpy(src, argv[1]);
    strcpy(dst, argv[2]);
    struct stat st;
    if (lstat(src, &st) == 0) {
        switch (st.st_mode & S_IFMT) {
            case S_IFREG: this.copyFile(src, dst); break;
            case S_IFDIR: this.copyDir(src, dst); break;
            case S_IFLNK: this.copySymLink(src, dst); break;
            default: break;
        }
    }
    else {
        printf("mycp: %s: No such file or directory\n", src);
    }
}

void ucp::setTime(char *path, struct stat st)
{
    struct utimbuf timebuf;
    timebuf.actime = st.st_atime;
    timebuf.modtime = st.st_mtime;
    if ((utime(path, &timebuf)) == -1) {
        printf("utime error.\n");
    }
}

void ucp::copyFile(char *src, char *dst)
{
    struct stat st;
    lstat(src, &st);
    if (dst[strlen(dst)-1] == '/') {
        strcat(dst, src);
    }
    int srcFile = open(src, O_RDONLY);
    int dstFile = open(dst, O_RDONLY);
    if (dstFile >= 0) {
        printf("ucp: %s: File already exits\n", dst);
        printf("Do you want to replace the existing file (yes/no)? ");
        char input[BUFFSIZE];
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
    char buf[BUFFSIZE];
    while ((n = read(srcFile, buf, BUFFSIZE)) > 0) {
        if (write(dstFile, buf, n) != n) {
            printf("write error\n");
            close(srcFile);
            close(dstFile);
            exit(-1);
        }
    }
    this.setTime(dst, st);

    close(srcFile);
    close(dstFile);
}

void ucp::walkDir(char *src, char *dst)
{
    struct dirent *entry;
    DIR* dir;
    if ((dir = opendir(src)) == NULL) {
        printf("opendir error\n");
        exit(1);
    }
    char srcpath[NAMESIZE], dstpath[NAMESIZE];

    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        strcpy(srcpath, src);
        strcat(srcpath, "/");
        strcat(srcpath, entry->d_name);
        strcpy(dstpath, dst);
        strcat(dstpath, "/");
        strcat(dstpath, entry->d_name);

        switch (entry->d_type) {
            case DT_REG: this.copyFile(srcpath, dstpath); break;
            case DT_DIR: this.copyDir(srcpath, dstpath); break;
            case DT_LNK: this.copySymLink(srcpath, dstpath); break;
            default: break;
        }
        //printf("%s %s\n", srcpath, dstpath);
    }
    closedir(dir);
}

void ucp::copyDir(char *src, char *dst)
{
    struct stat st;
    lstat(src, &st);
    DIR *srcDir = opendir(src);
    DIR *dstDir = opendir(dst);

    if (dstDir != NULL) {
        printf("mycp: %s: Directory already exits\n", dst);
        printf("Do you want to replace the existing Directory (yes/no)? ");
        char input[BUFFSIZE];
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
    this.walkDir(src, dst);
    this.setTime(dst, st);

}

void ucp::copySymLink(char *src, char *dst)
{
    struct stat st;
    lstat(src, &st);

    char buf[BUFFSIZE];
    int srcLink = readlink(src, buf, BUFFSIZE);
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

    struct timespec times[2] = {st.st_atim, st.st_mtim};
    int result = utimensat(AT_FDCWD, dst, times, AT_SYMLINK_NOFOLLOW);
}
