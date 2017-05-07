#ifndef _BUILTIN_FUNC_H
#define _BUILTIN_FUNC_H
#include "flag.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

class builtin_cmd {
public:
	virtual void run(int argc, char *argv[]) {}
};

// builtin command types
class ucp : public builtin_cmd
{
public:
	void run(int argc, char *argv[]);
	void setTime(char *path, struct stat st);
	void copyFile(char *src, char *dst);
	void walkDir(char *src, char *dst);
	void copyDir(char *src, char *dst);
	void copySymLink(char *src, char *dst);
};

class ucd : public builtin_cmd
{
public:
	void run(int argc, char *argv[]);
};

class test : public builtin_cmd {
public:
	void run(int argc, char *argv[]);
};

class utime : public builtin_cmd {
public:
	void run(int argc, char *argv[]);
};

#endif
