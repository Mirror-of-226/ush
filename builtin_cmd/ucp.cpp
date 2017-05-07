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
    if (dst[strlen(dst)-1] == '/') {
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
    struct timespec times[2] = {st.st_atim, st.st_mtim};
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
