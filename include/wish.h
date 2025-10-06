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

// function prototypes
void print_error(void);
char *trim_ws(char *s);
char **split_whitespace(char *line);

#endif
