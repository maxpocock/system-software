#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

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

        //load file
        if(strstr(client_message, "transfer")){
            transferFiles(socket_desc);
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

int transferFiles(int socket_desc){
    char fileName[30];
    char buff[5000];
    FILE *fp;
    printf("Please enter the name of the file you wish to transfer, including the file extension:\n");
    fgets(fileName, sizeof(fileName), stdin);
    fileName[strcspn(fileName, "\n")] = '\0';  // remove trailing newline
    
    fp = fopen(fileName,"r");
    if(fp==NULL){
        printf("Error while trying to open file, possible wrong name.");
        return -1;
    }

    while(fgets(buff, 5000, fp) != NULL) {
        if (send(socket_desc, buff, sizeof(buff), 0) == -1) {
        perror("[-]Error in sending file.");
        exit(1);
        }
        bzero(buff, 5000);
    }

    fclose(fp);
    return 0;
}
