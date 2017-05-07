#ifndef _PARSER_H_
#define _PARSER_H_
#include <stdio.h>
#include <stdlib.h>

class Parser
{
public:
    static char **run(char *line, int &num);
};
#endif
