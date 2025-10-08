/*
      Edited by Levi S. and Benjamin B.     10/6/2025
      Last edited by Mark W.    10/7/2025
 
*/

#ifndef WISH_H
#define WISH_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// shell prompt/error message
#define PROMPT "wish> "
#define ERRMSG "An error has occurred\n"

// prints the error message to stdout
static inline void printError(void) {
  ssize_t result = write(STDERR_FILENO, ERRMSG, strlen(ERRMSG));
  (void)result;
}


// function prototypes
void executeCommand(char **argv); // I had to add this for it to compile for me even though i didnt add this function. ~ Mark
void printError(void);
char *trimWs(char *s);
char **getPathDirs();
char **splitWhitespace(char *line);
int handleBuiltin(char **argv);
void initPath(void);
void freePath(void);

#endif
