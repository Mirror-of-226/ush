#include "../include/builtin_cmd.h"

void uls::run(int argc, char *argv[])
{
    if (argv[1] == NULL) {
        fprintf(stderr, "ush: expected argument to \"uls\"\n");
    }
    else {
        DIR *thedirectory;
        struct dirent *thefile;
        struct stat thestat;
        struct passwd *tf;
        struct group *gf;

        char buf[512];

        thedirectory = opendir(argv[1]);

        while ((thefile = readdir(thedirectory)) != NULL) {
            sprintf(buf, "%s/%s", argv[1], thefile->d_name);
            stat(buf, &thestat);

            switch (thestat.st_mode & S_IFMT) {
                case S_IFBLK: printf("b ");break;
                case S_IFCHR: printf("c ");break;
                case S_IFDIR: printf("d ");break;
                case S_IFIFO: printf("p ");break;
                case S_IFLNK: printf("l ");break;
                case S_IFSOCK:printf("s ");break;
                default:printf("- ");break;
            }

            printf((thestat.st_mode & S_IRUSR) ? " r" : " -");
            printf((thestat.st_mode & S_IWUSR) ? "w" : "-");
            printf((thestat.st_mode & S_IXUSR) ? "x" : "-");
            printf((thestat.st_mode & S_IRGRP) ? "r" : "-");
            printf((thestat.st_mode & S_IWGRP) ? "w" : "-");
            printf((thestat.st_mode & S_IXGRP) ? "x" : "-");
            printf((thestat.st_mode & S_IROTH) ? "r" : "-");
            printf((thestat.st_mode & S_IWOTH) ? "w" : "-");
            printf((thestat.st_mode & S_IXOTH) ? "x" : "-");

            printf("\t%d ", thestat.st_nlink);

            tf = getpwuid(thestat.st_uid);
            printf("\t%s ", tf->pw_name);

            gf = getgrgid(thestat.st_gid);
            printf("\t%s ", gf->gr_name);

            printf("%zu", thestat.st_size);
            printf(" %s", thefile->d_name);
            printf(" %s", ctime(&thestat.st_mtime));
        }
        closedir(thedirectory);
    }
}
