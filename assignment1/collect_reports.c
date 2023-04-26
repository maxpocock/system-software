/*
    This file is responsible for moving the files from the upload directory to the report directory and renaming them
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

void collect_reports(void) {

    pid_t pid = fork();

    if (pid == 0)
    {
        //child process
        time_t current = time(NULL);

        //converts current time into form that can be written to file
        char * time_str = ctime(&current);
        time_str[strlen(time_str)-1] = '\0';

        char date_file[19];
        char cmd[100];

        //date formatting
        strncpy(date_file, time_str, 10);
        date_file[3] = '_';
        date_file[7] = '_';

        char w[] = "warehouse";
        char m[] = "manufacturing";
        char s[] = "sales";
        char d[] = "distribution";

        chdir("/");
        chdir("/workspaces/system-software/assignment");
        //moving each report and renaming them
        sprintf(cmd, "mv upload/%s.xml report/%s_%s.xml", w, w, date_file);
        system(cmd);
        sprintf(cmd, "mv upload/%s.xml report/%s_%s.xml", m, m, date_file);
        system(cmd);
        sprintf(cmd, "mv upload/%s.xml report/%s_%s.xml", s, s, date_file);
        system(cmd);
        sprintf(cmd, "mv upload/%s.xml report/%s_%s.xml", d, d, date_file);
        system(cmd);
        return 0;
    }
    else if (pid > 0)
    {
        //parent process
        return 0;
    }
    else
    {
        printf("Something went wrong!!");
        return 1;
    }
}
