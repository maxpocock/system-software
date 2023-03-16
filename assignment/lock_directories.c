
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
	    chmod("upload", 0744);
        chmod("report", 0744);
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