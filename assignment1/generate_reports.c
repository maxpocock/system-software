
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>


void generate_reports(void) {
   chdir("/");
   chdir("/workspaces/system-software/assignment/upload");
   system("touch distribution.xml");
   system("touch manufacturing.xml");
   system("touch sales.xml");
   system("touch warehouse.xml");

   return 0;
}