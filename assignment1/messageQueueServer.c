/*
    This program creates a server that reads from a message queue and outputs to a log file
*/

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

int messageQueueServer(){

    time_t now;
    time(&now); 
    char * time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0';

    FILE *fp;

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

    do {
        mq = mq_open("/report_message_queue", O_CREAT | O_RDONLY, 0644, &queue_attributes);
        fp = fopen("/workspaces/system-software/assignment/logs/changes_log.txt", "a+");

        ssize_t bytes_read;

        /* receive message */
        bytes_read = mq_receive(mq, buffer, 1024, NULL);

        buffer[bytes_read] = '\0';
        if (! strncmp(buffer, "exit", strlen("exit")))
            { terminate = 1; }
        else
        {   
            printf("recieved %s \n", buffer);
            fwrite(time_str, strlen(time_str), 1, fp);
            fwrite("\n", strlen("\n"), 1, fp);
            fprintf(fp, "%s", buffer);
            fwrite("*******************\n", 20, 1, fp);
            fflush(fp);
        }
    } while (!terminate);

    mq_close(mq);
    mq_unlink("/report_message_queue");
    fclose(fp);

    return 0;
}

