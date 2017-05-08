#include "../include/builtin_cmd.h"

#define MAX 1024
#define PATH_SIZE 128

void ups::run(int argc, char **argv) {

#if defined(__linux__) || defined(linux) || defined(__linux)
    DIR *dir;
    struct dirent *entry;
    FILE *fp;
    char path[PATH_SIZE];
    char buf[MAX];
    printf("NAME\t\tPID\n");
    if((dir = opendir("/proc")) == NULL ) {
        perror("fail to open dir");
        return;
    }
    while((entry = readdir( dir ) ) != NULL){
        if(entry->d_name[0] == '.')
            continue;
        if( (entry->d_name[0] <='0' ) || (entry->d_name[0] >= '9'))
            continue;
        sprintf(path, "/proc/%s/task/%s/status", entry->d_name,entry->d_name);
        fp = fopen(path, "r");
        if(fp == NULL){
            perror("fail to open");
            exit(1);
        }
        while(fgets(buf, MAX, fp) != NULL){
            if(buf[0] == 'N'
               && buf[1] == 'a'
               && buf[2] == 'm'
               && buf[3] == 'e')
            {
                int i=6;
                while(buf[i]!='\n')
                {
                    printf("%c", buf[i]);
                    i++;
                }
            }
            if(buf[0] == 'P'
               && buf[1] == 'i'
               && buf[2] == 'd'){
                printf("\t\t%s", &buf[5]);
                break;
            }
        }
        fclose(fp);
    }
    closedir(dir);
#endif

#if defined(__APPLE__)  || defined(__MACH__)
    system("ps");
#endif
}