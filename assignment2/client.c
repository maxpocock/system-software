#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/types.h>


int transferFiles(int socket_desc);

int main(void)
{
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[100], client_message[100];
    
    // Clean buffers:
    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_desc < 0){
        printf("Unable to create socket\n");
        return -1;
    }
    
    printf("Socket created successfully\n");
    
    // Set port and IP the same as server-side:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Send connection request to server:
    if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Unable to connect\n");
        return -1;
    }
    printf("Connected with server successfully\n");
    
    while(!strstr(client_message, "exit")){
        // Get input from the user:
        printf("Enter message: ");
        fgets(client_message, sizeof(client_message), stdin);
        
        // Send the message to server:
        if(send(socket_desc, client_message, strlen(client_message), 0) < 0){
            printf("Unable to send message\n");
            return -1;
        }
        
        // Receive the server's response:
        if(recv(socket_desc, server_message, sizeof(server_message), 0) < 0){
            printf("Error while receiving server's msg\n");
            return -1;
        }
        
        printf("Server's response: %s \n", server_message);

        client_message[strcspn(client_message, "\n")] = 0;

        //load file
        if(strstr(client_message, "transfer")){
            transferFiles(socket_desc);
        }

        if (strcmp(client_message, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        // Clean buffers:
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
    }
    printf("Thank You, the program will now exit.\n");
    // Close the socket:
    close(socket_desc);
    
    return 0;
}

int transferFiles(int socket_desc) {
    char fileName[30];
    char buff[1024];
    FILE *fp;
    uid_t uid = getuid();
    gid_t gid = getgid();
    printf("Please enter the name of the file you wish to transfer, including the file extension:\n");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';

    //sending user id to server
    if (send(socket_desc, &uid, sizeof(uid), 0) < 0) {
        printf("Error sending user id.\n");
        return -1;
    }

    //sending group id to server
    if (send(socket_desc, &gid, sizeof(gid), 0) < 0) {
        printf("Error sending group id.\n");
        return -1;
    }
    
    //open file to be sent
    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("Error while trying to open file, possible wrong name.\n");
        return -1;
    }

    puts(fileName);
    // Send file name to server:
    if (send(socket_desc, &fileName, sizeof(fileName), 0) < 0) {
        printf("Error sending file name.\n");
        return -1;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Send file size to server
    if (send(socket_desc, &file_size, sizeof(file_size), 0) < 0) {
        printf("Error sending file size.\n");
        return -1;
    }

    // Send file data to server:
    size_t bytes_read;
    while ((bytes_read = fread(buff, 1, sizeof(buff), fp)) > 0) {
        if (send(socket_desc, buff, bytes_read, 0) < 0) {
            printf("Error sending file data.\n");
            return -1;
        }
    }

    fclose(fp);

    return 0;
}
