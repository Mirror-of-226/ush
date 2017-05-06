#ifndef _BUILTIN_FUNC_H
#define _BUILTIN_FUNC_H

class builtin_cmd {
    virtual run(int argc, char *argv[]) = 0;
}

//builtin command types
class ucp: public builtin_cmd
{
public:
    void run(int argc, char *argv[]);


    void setTime(char *path, struct stat st);


    void copyFile(char *src, char *dst);
   

    void walkDir(char *src, char *dst);
    

    void copyDir(char *src, char *dst);
   

    void copySymLink(char *src, char *dst);
    
};
