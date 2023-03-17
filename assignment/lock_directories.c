
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>

void lock_directories() {
    pid_t pid = fork();

    if (pid == 0)
    {
        //child process
        //navigate to root directory
        chdir("/");
        //navigate to assignment directory
        chdir("/workspaces/system-software/assignment");
        //call chmod to change directory permissions so data cannot be read or written by users or groups
        system("chmod 700 upload");
        system("chmod 700 report");
    }
    else if (pid > 0)
    {
        //parent process
    }
    else
    {
        printf("Something went wrong!!");
        return;
    }

    return;
}