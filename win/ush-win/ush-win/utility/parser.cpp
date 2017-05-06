#include "../include/parser.h"
#include "../include/ush.h"

#define USH_TOK_DELIM " \t\r\n\a"

char **Parser::run(char* line, int& num) {
    int bufsize = BUF_SIZE;
    int index = 0;
    char **tokens = (char **)malloc(bufsize * sizeof(char*));
    char *token, **tokens_backup;

    if (!tokens) {
        fprintf(stderr, "ush: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, USH_TOK_DELIM);

    while (token != NULL) {
        tokens[index] = token;
        index++;

        if (index >= bufsize) {
            bufsize += BUF_SIZE;
            tokens_backup = tokens;
            tokens = (char**)realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                free(tokens_backup);
                fprintf(stderr, "ush: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, USH_TOK_DELIM);
    }
    tokens[index] = NULL;
	num = index; // 0 -> index - 1
    return tokens;
}
