
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>

void unlock_directories() {
    pid_t pid = fork();

    if (pid == 0)
    {
        //child process
	    chmod("upload", 0766);
        chmod("reporting", 0744);
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
        