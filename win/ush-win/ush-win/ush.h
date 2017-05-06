#pragma once

class ush
{
	static const int INPUT_BUF_SIZE = 1024;
	char input_buf[INPUT_BUF_SIZE];
	//map<string, int> MBF; // mapping of builtin function
	void loadMBF();
public:
	ush();
	~ush();
};

