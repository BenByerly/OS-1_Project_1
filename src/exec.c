/*
      Edited by Levi S. and Benjamin B.     10/6/2025
      Edited by Mark W.    10/7/2025
      Last edited by Gage J.    10/12/2025
 
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

// Execute multiple command strings in parallel. Each entry in commands is a
// null-terminated command string (not tokenized). This function forks a child
// for each command, runs the command in the child, and the parent waits for
// all children to finish before returning.
void executeParallelCommands(char **commands) {
    // Count commands
    int count = 0;
    while (commands[count]) count++;
    if (count == 0) return;

    pid_t *pids = malloc(count * sizeof(pid_t));
    if (!pids) {
        printError();
        return;
    }

    for (int i = 0; i < count; i++) {
        // For each command, split into argv and execute in child
        char *cmd_copy = strdup(commands[i]);
        if (!cmd_copy) {
            printError();
            pids[i] = -1;
            continue;
        }
        char **argvv = splitWhitespace(cmd_copy);
        if (!argvv || !argvv[0]) {
            // empty command, skip
            for (int j = 0; argvv && argvv[j]; j++) free(argvv[j]);
            free(argvv);
            free(cmd_copy);
            pids[i] = -1;
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            printError();
            pids[i] = -1;
        } else if (pid == 0) {
            // child: execute the command
            executeCommand(argvv);
            // cleanup in child
            for (int j = 0; argvv[j]; j++) free(argvv[j]);
            free(argvv);
            free(cmd_copy);
            _exit(0);
        } else {
            // parent: remember child's pid
            pids[i] = pid;
            // free argv memory in parent
            for (int j = 0; argvv[j]; j++) free(argvv[j]);
            free(argvv);
            free(cmd_copy);
        }
    }

    // Wait for all children we started
    for (int i = 0; i < count; i++) {
        if (pids[i] > 0) {
            int status;
            waitpid(pids[i], &status, 0);
        }
    }

    free(pids);
}
