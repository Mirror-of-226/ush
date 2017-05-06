#ifndef _PARSER_H_
#define _PARSER_H_
#include <stdio.h>

class Parser
{
public:
    static char **run(char *line, int &num);
};
#endif