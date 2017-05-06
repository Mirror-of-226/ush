#pragma once
#include <stdio.h>
#include <map>
#include "..\include\builtin_cmd.h"
#include <vector>

#define BUF_SIZE 1024
#define INPUT_BUF_SIZE 1024

class ush
{
	char input_buf[INPUT_BUF_SIZE];
	std::map<std::string, builtin_cmd*> MBC; // mapping of builtin command
	void loadMBF();
public:
	ush();
	~ush();
};

