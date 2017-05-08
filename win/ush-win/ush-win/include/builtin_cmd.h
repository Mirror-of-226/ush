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
	char *now_path;
public:
	ucd(char *_now_path);
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

class uls : public builtin_cmd {
	char *now_path;
public:
	uls(char *_now_path);
	void run(int argc, char *argv[]);
};

class upwd : public builtin_cmd {
	char *now_path;
public:
	upwd(char *_now_path);
	void run(int argc, char *argv[]);
};

class urm : public builtin_cmd {
	char *now_path;
public:
	urm(char *_now_path);
	void run(int argc, char *argv[]);
};

#endif
