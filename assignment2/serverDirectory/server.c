#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>

pthread_mutex_t lock_x; 

void *addThread(void *client_sock);

int main(void)
{
    pthread_t threads[10];
    int socket_desc, client_sock, client_size, iret, counter = 0;
    struct sockaddr_in server_addr, client_addr;

    //create lock
    pthread_mutex_init(&lock_x, NULL);
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_desc < 0) {
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Bind to the set port and IP:
    if (bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("Couldn't bind to the port\n");
        return -1;
    }
    printf("Done with binding\n");
    
    // Listen for clients:
    if (listen(socket_desc, 1) < 0) {
        printf("Error while listening\n");
        return -1;
    }
    printf("\nListening for incoming connections.....\n");
    
    while (1) {
        // Accept an incoming connection:
        client_size = sizeof(client_addr);
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
        
        if (client_sock < 0) {
            printf("Can't accept\n");
            return -1;
        }

        iret = pthread_create(&threads[counter], NULL, addThread, (void*)client_sock);
        if (iret) {
            fprintf(stderr, "Error - pthread_create() return code: %d\n", iret);
            exit(EXIT_FAILURE);
        }
        
        printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        if (counter == 9) {
            counter = 0;
        } else {
            counter++;
        }
    }
    
    // Closing the socket:
    close(socket_desc);
    
    return 0;
}

void *addThread(void *client_sock){
    int read_size;
    char client_message[100], file_name[30], buff[1024], cmd[100], directory[30];
    char success[] = "Successful";
    char transfer[] = "Transfer Started";
    int groupID[6], userID[6];
    size_t file_size;
    int uid;
    int gid;
    FILE *fp;
    while((read_size = recv(client_sock , client_message , 2000 , 0)) > 0 ){
        // Receive client's message:
        if (read_size < 0){
            printf("Couldn't receive\n");
            return NULL;
        }
        printf("Msg from client: %s\n", client_message);
        
        // Respond to client:        
        if (send(client_sock, transfer, strlen(transfer), 0) < 0){
            printf("Can't send\n");
            return NULL;
        }

        if(strstr(client_message, "transfer")){

            memset(client_message,'\0',sizeof(client_message));

            //receive user id
            recv(client_sock, &uid, sizeof(uid), 0);

            //receive group id
            recv(client_sock, &gid, sizeof(gid), 0);
            printf("Group ID: %d \n", gid);

            memset(directory,'\0',sizeof(directory));
            
            // Receive directory name from client:
            recv(client_sock, directory, sizeof(directory), 0);

            memset(file_name,'\0',sizeof(file_name));
            
            // Receive file name from client:
            recv(client_sock, &file_name, sizeof(file_name), 0);

            // get access to lock 
            pthread_mutex_lock(&lock_x);

            // Receive file size from client:
            size_t file_size;
            recv(client_sock, &file_size, sizeof(file_size), 0);

            // Open file for writing:
            sprintf(cmd, "%s/%s", directory, file_name);
            FILE *fp = fopen(cmd, "wb");
            if (fp == NULL) {
                printf("Error opening file.\n");
                //release lock
                pthread_mutex_unlock(&lock_x);
                return -1;
            }

            //attributing file to the user 
            int fileid = fileno(fp);
            if (fchown(fileid, uid, gid) == -1) {
                printf("Error changing file owner\n");
                //release lock
                pthread_mutex_unlock(&lock_x);
                return -1;
            }

            // Receive file data from client:
            char buffer[1024];
            size_t total_bytes = 0;
            while (total_bytes < file_size) {
                size_t bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0) {
                    printf("Error receiving file data.\n");
                    //release lock
                    pthread_mutex_unlock(&lock_x);
                    return NULL;
                }
                if (fwrite(buffer, 1, bytes_received, fp) != bytes_received) {
                    printf("Error writing to file.\n");
                    //release lock
                    pthread_mutex_unlock(&lock_x);
                    return NULL;
                }
                total_bytes += bytes_received;
            }
            fclose(fp);
                    
            if (send(client_sock, success, strlen(success), 0) < 0){
                printf("Can't send\n");
                return NULL;
            }
            puts(success);

            //release lock
            pthread_mutex_unlock(&lock_x);
        }
    }
    close(client_sock);
    return NULL;
}
