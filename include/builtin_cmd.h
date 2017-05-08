#ifndef _BUILTIN_FUNC_H
#define _BUILTIN_FUNC_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>

#if defined(__APPLE__)  || defined(__MACH__) || defined(__linux__) || defined(linux) || defined(__linux)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <utime.h>
#endif

class builtin_cmd {
public:
	virtual void run(int argc, char *argv[]) {}
};

// builtin command types
class ucp : public builtin_cmd
{
public:
	void run(int argc, char *argv[]);
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


class umkdir : public builtin_cmd {
public:
	void run(int argc, char *argv[]);
};

class upwd : public builtin_cmd {
public:
	void run(int argc, char *argv[]);
};

class uecho : public builtin_cmd {
public:
    void run(int argc, char *argv[]);
};

class uhelp : public builtin_cmd {
	std::map<std::string, builtin_cmd*>* mbc;
public:
	uhelp(std::map<std::string, builtin_cmd*>& _mbc);
	void run(int argc, char *argv[]);
};

#endif
