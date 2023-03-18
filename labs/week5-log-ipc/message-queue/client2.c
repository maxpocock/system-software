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
    ssize_t bytes_sent;

    // Open the message queue
    mq = mq_open("/dt228_queue", O_WRONLY);
    if (mq == (mqd_t) -1) {
        perror("mq_open");
        exit(1);
    }

    // Read messages from standard input and send them to the message queue
    while (fgets(msg_buffer, MSG_SIZE, stdin)) {
        bytes_sent = mq_send(mq, msg_buffer, sizeof(msg_buffer), 0);
        if (bytes_sent == -1) {
            perror("mq_send");
            continue;
        }
    }

    // Close the message queue
    mq_close(mq);

    return 0;
}
