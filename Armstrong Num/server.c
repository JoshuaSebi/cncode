//TIME SERVER USING TCP

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
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
    struct sockaddr_in serveraddr,clientaddr;

    sock=socket(AF_INET,SOCK_STREAM,0);
    errorcheck(sock, "Socket Created");

    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_addr.s_addr=LOCALHOST;

    int b=bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    errorcheck(b,"Binding Done");

    int l=listen(sock,5);
    errorcheck(l,"Listening...");

    char buffer[MAXSIZE];
    while(1){
        socklen_t clen=sizeof(clientaddr);
        int newsock = accept(sock,(struct sockaddr*)&clientaddr, &clen);
        errorcheck(newsock,"Connection Established!!!\n");

        int status=recv(newsock,buffer,MAXSIZE,0);
        printf("Number from Client: %s\n", buffer);

        int num=atoi(buffer);

        int temp=num,sum=0;
        while(temp>0){
            int x=temp%10;
            sum+=(x*x*x);
            temp=temp/10;
        }

        char msg[30];
        if(num==sum){
            sprintf(msg,"Armstrong Num");
        } else {
            sprintf(msg,"Not Armstrong");
        }
        send(newsock,msg,strlen(msg),0);
        printf("Reply Sent\n");
    }
    

    close(sock);
}