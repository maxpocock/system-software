#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#define MSG_QUEUE_NAME "/my_msg_queue"
#define MSG_SIZE 1024

int main() {
    mqd_t mq;
    char msg_buffer[MSG_SIZE];
    int msg_priority;
    ssize_t bytes_read;

    struct mq_attr queue_attributes;
    char buffer[1024 + 1];
    int terminate = 0;

    /* set queue attributes */
    queue_attributes.mq_flags = 0;
    queue_attributes.mq_maxmsg = 10;
    queue_attributes.mq_msgsize = 1024;
    queue_attributes.mq_curmsgs = 0;


    // Open the message queue
    mq = mq_open("/dt228_queue", O_CREAT | O_RDONLY, 0644, &queue_attributes);
        if (mq == (mqd_t) -1) {
        perror("mq_open");
        exit(1);
    }

    // Open the output file for writing
    FILE *output_file = fopen("changes_log.txt", "a+");
    if (output_file == NULL) {
        perror("fopen");
        exit(1);
    }

    // Read messages from the message queue and write them to the output file
    while (1) {
        // Wait for a message to arrive
        bytes_read = mq_receive(mq, msg_buffer, MSG_SIZE, &msg_priority);
        if (bytes_read == -1) {
            perror("mq_receive");
            continue;
        }
        
        msg_buffer[bytes_read] = '\0';

        // Write the message to the output file
        fwrite(msg_buffer, 1, bytes_read, output_file);
        msg_buffer = [""];
        fflush(output_file);
    }

    // Close the message queue and output file
    mq_close(mq);
    fclose(output_file);

    return 0;
}
