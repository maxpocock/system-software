#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <unistd.h>
#include <syslog.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

int main()
{
    FILE * fp;
    mqd_t mq;
    struct mq_attr queue_attributes;
    char buffer[1024 + 1];
    int terminate = 0;

    /* set queue attributes */
    queue_attributes.mq_flags = 0;
    queue_attributes.mq_maxmsg = 10;
    queue_attributes.mq_msgsize = 1024;
    queue_attributes.mq_curmsgs = 0;

    /* create queue */
    mq = mq_open("/dt228_queue", O_CREAT | O_RDONLY, 0644, &queue_attributes);
 
    fp = fopen("/workspaces/system-software/assignment/logs/changes_log.txt", "a+");
    do {
        ssize_t bytes_read;

        /* receive message */
        bytes_read = mq_receive(mq, buffer, 1024, NULL);

        buffer[bytes_read] = '\0';
        if (! strncmp(buffer, "exit", strlen("exit")))
        { terminate = 1; }
        else
        { 
            fprintf(fp, "%s", buffer);
            printf("Received: %s\n", buffer); 
            }
    } while (1 != 0);

    mq_close(mq);
    mq_unlink("/dt228_queue");
    return 0;

}

