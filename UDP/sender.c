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
    struct sockaddr_in clientaddr;
    char buffer[BUFFER_SIZE];

    //create Socket
    sock= socket(AF_INET, SOCK_DGRAM,0);
    error_check(sock, "Socket created");

    //configure client address
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port=htons(PORT);
    clientaddr.sin_addr.s_addr=LOCALHOST;
    socklen_t clen=sizeof(clientaddr);

    //operations
    int status=-1;
    char msg[BUFFER_SIZE];
    printf("Enter a message:");
    fgets(msg, BUFFER_SIZE, stdin);

    status=sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*) &clientaddr, clen);
    error_check(status, "Message sent");

    memset(buffer, BUFFER_SIZE, 0);
    status= recvfrom(sock, msg, sizeof(msg)-1, 0, (struct sockaddr*) &clientaddr, &clen);
    error_check(status, "Reply received");
    printf("Server: %s\n", msg);

    //close
    close(sock);

}