
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {
    
    struct stat file_stat;

    //printf("st_mtime = %i \n", file_stat.st_uid);
    
    FILE *fp;
    FILE *fp1;
    int status;
    char path[1024];
    char stats[1060];

    fp1 = fopen("/workspaces/system-software/assignment/logs/uploads_log.txt", "a+");
    fp = popen("ls upload", "r");

   while (fgets(path, 1024, fp) != NULL){

        stat(path, &file_stat);

        sprintf(stats, "-- created on %li \n", file_stat.st_mtime);

        fwrite(path, strlen(path), 1, fp1);
        fwrite(stats, strlen(stats), 1, fp1);
    }
    fclose(fp);  
    fclose(fp1);
   //status = pclose(fp);

   return 0;

}