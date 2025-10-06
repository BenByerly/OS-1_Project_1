#include "wish.h"
 
// remove leading and trailing whitespace
char *trimWs(char *s) {
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
 
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) end--;
    *(end + 1) = 0;
    return s;
}
 
// split a line into tokens (command + args)
char **splitWhitespace(char *line) {
    int bufsize = 64;
    char **tokens = malloc(bufsize * sizeof(char *));
    if (!tokens) {
        printError();
        exit(1);
    }
 
    int pos = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL) {
        tokens[pos++] = strdup(token);
        if (pos >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                printError();
                exit(1);
            }
        }
        token = strtok(NULL, " \t\n");
    }
    tokens[pos] = NULL;
    return tokens;
}
