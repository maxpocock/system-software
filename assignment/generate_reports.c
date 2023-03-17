
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>


void generate_reports(void) {
    printf("Generate report function could go here.. you do not have to follow this design but an option if you are stuck");
    
    struct stat file_stat;
    stat("distribution.xml", &file_stat);

    printf("\n st_mtime = %li", file_stat.st_mtime);
    
    FILE *fp;
    int status;
    char path[1024];

    fp = popen("ls report", "r");

   while (fgets(path, 1024, fp) != NULL)
      printf("%s", path);

   status = pclose(fp);

}