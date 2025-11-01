#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 1292
#define LOCALHOST inet_addr("127.0.0.1")

typedef struct{
    int fnum;
} Frame;

typedef struct{
    int anum;
} Ack;

void main(){
    int sock;
    struct sockaddr_in serveraddr, clientaddr;
    Frame f;
    Ack a;

    sock=socket(AF_INET, SOCK_DGRAM, 0);

    socklen_t clen=sizeof(clientaddr);
    serveraddr.sin_addr.s_addr=LOCALHOST;
    serveraddr.sin_port=PORT;
    serveraddr.sin_family=AF_INET;
    bind(sock,(struct sockaddr*)&serveraddr, sizeof(serveraddr) );

    int exp=1;
    int drop=0;
    while(exp<=10){
        recvfrom(sock, &f, sizeof(f),0,(struct sockaddr*)&clientaddr,&clen);
        printf("[S] Frame %d Recvd\n",f.fnum);

        if(f.fnum%3==0 && drop==0){
            drop=1;
            printf("[S] 3 num frames intentionally dropped\n");
            continue;
        }

        if(f.fnum==exp){
            a.anum=f.fnum;
            printf("[S] Ack sent for Frame %d\n",a.anum);
            sendto(sock, &a, sizeof(a),0,(struct sockaddr*)&clientaddr,clen);
            drop=0;
            exp++;
        } else {
            a.anum=exp-1;
        }
        sleep(1);
    }
    close(sock);
}