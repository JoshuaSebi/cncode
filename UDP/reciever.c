#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 3969
#define LOCALHOST inet_addr("127.0.0.1")
#define BUFFER_SIZE 256

void error_check(int s, char msg[]){
    if (s<0){
        perror("Failed");
        exit(0);
    } else {
        printf("%s\n", msg);
    }
}

void main(){
    //define variables
    int sock;
    struct sockaddr_in serveraddr, clientaddr;
    char buffer[BUFFER_SIZE];

    //create socket
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    error_check(sock, "Socket created");

    //bind the server address
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=LOCALHOST;
    int b= bind(sock, (struct sockaddr*) & serveraddr, sizeof(serveraddr));
    error_check(b, "Binding done");
    
    //operations
    socklen_t clen= sizeof(clientaddr);
    while(1){
        int status=-1;
        memset(buffer,BUFFER_SIZE,0);
        char msg[BUFFER_SIZE];

        status=recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*) &clientaddr, &clen);
        error_check(status, "Message received");
        printf("Client: %s\n", buffer);

        printf("Enter a message:");
        fgets(msg, BUFFER_SIZE, stdin);
        status=sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*) &clientaddr, clen);
        error_check(status, "Reply sent");
    }

    close(sock);
}