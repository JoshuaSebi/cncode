//TIME SERVER USING TCP

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 256
#define PORT 2310
#define LOCALHOST inet_addr("127.0.0.1")

void errorcheck(int x, char msg[]){
    if(x<0){
        perror("Error Occurred");
        exit(0);
    } else {
        printf("%s\n",msg);
    }
}

void main(){
    int sock;
    struct sockaddr_in serveraddr;

    sock=socket(AF_INET,SOCK_STREAM,0);
    errorcheck(sock, "Socket Created.");

    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=LOCALHOST;

    int c=connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    errorcheck(c,"Connection Established");

    char buffer[MAXSIZE];
    printf("Requesting to Check Armstrong: \n");
    fgets(buffer, MAXSIZE, stdin);

    int status=send(sock, buffer,strlen(buffer),0);
    errorcheck(status, "Request Sent to Server...");

    memset(buffer,0,MAXSIZE);
    status=recv(sock, buffer,MAXSIZE,0);
    buffer[status]='\0';
    printf("Result: %s",buffer);

    close(sock);
}