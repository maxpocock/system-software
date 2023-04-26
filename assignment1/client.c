/*
    This client is responsible for documenting changes to the upload directory
*/

#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    mqd_t mq;
    char buffer[1024];

    /* open the message queue */
    mq = mq_open("/report_message_queue", O_WRONLY);

    printf("Enter file name, create or modify and username:\n");

    
    printf(">> ");
    fflush(stdout);

    memset(buffer, 0, 1024);
    fgets(buffer, 1024, stdin);
    mq_send(mq, buffer, 1024, 0);
    printf("Thank you, this program will close now\n ");

    mq_close(mq); 
    return 0;
}
