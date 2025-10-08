/*
      Edited by Levi S. and Benjamin B.     10/6/2025
      Last edited by Mark W.    10/7/2025
 
*/


// exec.c
#include "wish.h"
#include <fcntl.h>   // added for open(), O_WRONLY, etc.
#include <sys/wait.h> // added for waitpid()

void executeCommand(char **argv) {
    char **pathDirs = getPathDirs();

    if (!pathDirs || !pathDirs[0]) {
        printError();
        return;
    }

    // Added redirection detection for operator ">" ~ Mark
    int redirectIndex = -1;
    char *outfile = NULL;
    for (int i = 0; argv[i]; i++) {
        if (strcmp(argv[i], ">") == 0) {
            redirectIndex = i;
            break;
        }
    }

    if (redirectIndex != -1) {
        // Error if no filename or too many args
        if (!argv[redirectIndex + 1] || argv[redirectIndex + 2]) {
            printError();
            return;
        }
        outfile = argv[redirectIndex + 1];
        argv[redirectIndex] = NULL; // Terminate command args
    }
    // End of redirection detection

    pid_t pid = fork();

    if (pid < 0) {
        printError();
        return;
    }

    if (pid == 0) {
        // Added output redirection handling ~ Mark
        if (outfile) {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
            if (fd < 0) {
                printError();
                _exit(1);
            }
            dup2(fd, STDOUT_FILENO);  // redirect stdout
            dup2(fd, STDERR_FILENO);  // redirect stderr
            close(fd);
        }
        // End of redirection handling

        for (int i = 0; pathDirs[i]; i++) {
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", pathDirs[i], argv[0]);
            if (access(path, X_OK) == 0) {
                execv(path, argv);
                printError();
                _exit(1);
            }
        }

        // if no path worked command not found error given
        printError();
        _exit(1);
    } else {
        // parent waits for child to finish
        int status;
        waitpid(pid, &status, 0);
    }
}
