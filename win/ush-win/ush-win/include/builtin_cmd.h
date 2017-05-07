#pragma once
#ifndef _BUILTIN_FUNC_H
#define _BUILTIN_FUNC_H
#include <map>

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

class ups : public builtin_cmd {
public:
	void run(int argc, char *argv[]);
};

class ukill : public builtin_cmd {
public:
	void run(int argc, char *argv[]);
};

class utype : public builtin_cmd {
	std::map<std::string, builtin_cmd*>* mbc;
public:
	utype(std::map<std::string, builtin_cmd*>& _mbc);
	void run(int argc, char *argv[]);
};

#endif
