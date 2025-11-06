#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3025
#define BUFFER_SIZE 256
#define LOCALHOST inet_addr("127.0.0.1")

void main(){

    //define variables
    int sock;
    struct sockaddr_in client_addr;
    char buffer[BUFFER_SIZE];

    //error check
    void error_check(int s, char msg[]){
        if (s<0){
            perror("Failed");
            exit(0);
        }
        else{
            printf("\n%s\n",msg);
        }
    }

    //create socket
    sock = socket(AF_INET, SOCK_STREAM,0);
    error_check(sock,"Socket Created");


    client_addr.sin_family=AF_INET;
    client_addr.sin_port=htons(PORT);
    client_addr.sin_addr.s_addr=LOCALHOST;
    socklen_t len1 = sizeof(client_addr);

    //connect
    int conn = connect(sock, (struct sockaddr *) &client_addr, len1);
    error_check(conn, "Connection Established");

    //Operations
    int status = -1;
    char msg[] = "Hello from client";

    status = send(sock, msg, sizeof(msg), 0);
    error_check(status, "Message Sent");

    status = recv(sock,buffer,BUFFER_SIZE,0);
    error_check(status, "Reply Received");
    printf("Server: %s\n", buffer);

    //close
    close(sock);
}