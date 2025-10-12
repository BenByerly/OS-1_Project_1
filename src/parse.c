/*
    Last edited by Levi S. and Benjamin B.     10/6/2025
    Last edited by Gage J.    10/12/2025
*/


// parse.c
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

// split a line by '&' into trimmed command strings
char **splitByAmpersand(char *line) {
    int bufsize = 8;
    char **commands = malloc(bufsize * sizeof(char *));
    if (!commands) {
        printError();
        exit(1);
    }

    int pos = 0;
    char *saveptr = NULL;
    char *token = strtok_r(line, "&", &saveptr);
    while (token != NULL) {
        char *trimmed = trimWs(token);
        commands[pos++] = strdup(trimmed);
        if (pos >= bufsize) {
            bufsize *= 2;
            commands = realloc(commands, bufsize * sizeof(char *));
            if (!commands) {
                printError();
                exit(1);
            }
        }
        token = strtok_r(NULL, "&", &saveptr);
    }
    commands[pos] = NULL;
    return commands;
}
