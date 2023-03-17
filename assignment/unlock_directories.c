
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
        //navigate to root directory
        chdir("/");
        //navigate to assignment directory
        chdir("/workspaces/system-software/assignment");
        //call chmod to change directory permissions so data cannot be read or written by users or groups
        system("chmod 777 upload");
        system("chmod 744 report");
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
        