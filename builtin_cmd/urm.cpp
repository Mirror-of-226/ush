#include "../include/builtin_cmd.h"
#include "../include/ush.h"
#include <iostream>

int rmdir(std::string dir_full_path)
{
    DIR* dirp = opendir(dir_full_path.c_str());
    if(!dirp)
    {
        return -1;
    }
    struct dirent *dir;
    struct stat st;
    while((dir = readdir(dirp)) != NULL)
    {
        if(strcmp(dir->d_name,".") == 0
           || strcmp(dir->d_name,"..") == 0)
        {
            continue;
        }
        std::string sub_path = dir_full_path + '/' + dir->d_name;
        if(lstat(sub_path.c_str(),&st) == -1)
        {
            continue;
        }
        if(S_ISDIR(st.st_mode))
        {
            if(rmdir(sub_path) == -1)
            {
                closedir(dirp);
                return -1;
            }
            rmdir(sub_path.c_str());
        }
        else if(S_ISREG(st.st_mode))
        {
            unlink(sub_path.c_str());
        }
        else
        {
            continue;
        }
    }
    if(rmdir(dir_full_path.c_str()) == -1)
    {
        closedir(dirp);
        return -1;
    }
    closedir(dirp);
    return 0;
}

void urm::run(int argc, char **argv) {
    if (argv[1] == NULL) {
        fprintf(stderr, "ush: ebufpected argument to \"urm\"\n");
    } else {
        char path[BUF_SIZE];
        strcpy(path, argv[1]);

        struct stat st;
        if (lstat(path, &st) == -1) {
            perror("ush");
        }
        if (S_ISREG(st.st_mode)) {
            if (unlink(path) == -1) {
                return;
            }
        }
        else if (S_ISDIR(st.st_mode)) {
            if (strcmp(path, ".") == 0 || strcmp(path, ",") == 0) {
                return;
            }
            if (rmdir(path) == -1) {
                return;
            }
        }
    }
}