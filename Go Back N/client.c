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
#define WIN 3 

typedef struct{
    int fnum;
} Frame;

typedef struct{
    int anum;
} Ack;

void main(){
    int sock;
    struct sockaddr_in serveraddr;
    struct timeval tv={4,0};
    Ack a;
    Frame f;

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=LOCALHOST;
    socklen_t clen=sizeof(serveraddr);

    int base=1, next=1,s2=0;
    while(base<=7){
        while(next<=(base+WIN-1) && next<=7){
            f.fnum=next;
            printf("[C] Frame %d sent\n",next);
            sendto(sock, &f, sizeof(f),0,(struct sockaddr*)&serveraddr,clen);
            while(next==2 && s2==0){
                s2=1;
                printf("[C] Frame %d dup sent\n",next);
                sendto(sock, &f, sizeof(f),0,(struct sockaddr*)&serveraddr,clen);
                continue;
            }
            next++;
        }

        int n=recvfrom(sock, &a, sizeof(a),0,(struct sockaddr*)&serveraddr,&clen);
        if(n<0){
            printf("[C] NACK - Frames resent\n");
            next=base;
        } else if (a.anum>=base){
            printf("[C] ACK %d recv\n",a.anum);
            base=a.anum+1;
        }
    }
}