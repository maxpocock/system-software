#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void backup_dashboard(void) {
    chdir("/");
    chdir("/workspaces/system-software/assignment/");
    //clear backup folder
    execl("/backup", "rm", NULL);
    //copy everything from report directory to backup directory
    system("cp -a report/. /workspaces/system-software/assignment/backup/");
    return 0;
}
