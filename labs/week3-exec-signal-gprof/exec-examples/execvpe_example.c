#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *argv[] = { "sh", "-c", "env", 0 };
    char *envp[] =
    {
        "HOME=/",
        "PATH=//workspaces/system-software/labs/week3-exec-signal-gprof/exec-examples",
        "TZ=UTC0",
        "USER=jmccarthy",
        "LOGNAME=logfile.txt",
    };
    execvpe(argv[0], &argv[0], envp);
    fprintf(stderr, "Error.....!\n");
    return -1;
}