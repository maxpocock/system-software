#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syslog.h>

void collect_reports(void) {
        char *argv[] = { "cp ./upload", "./reporting", NULL };

    execv(argv[0], argv[1]);
}
