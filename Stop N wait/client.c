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
    struct sockaddr_in serveraddr;
    Frame f;
    Ack a;

    sock=socket(AF_INET, SOCK_DGRAM, 0);

    serveraddr.sin_addr.s_addr=LOCALHOST;
    serveraddr.sin_port=PORT;
    serveraddr.sin_family=AF_INET;

    struct timeval tv={3,0};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    socklen_t clen=sizeof(serveraddr);
    for (int i=1; i<=10;i++){
        f.fnum=i;
        printf("[C] Frame %d sent\n",i);

        while(1){
            sendto(sock, &f, sizeof(f),0,(struct sockaddr*)&serveraddr,clen);

            //wait for ack
            int n=recvfrom(sock, &a, sizeof(a),0,(struct sockaddr*)&serveraddr,&clen);
            if(n>=0){
                printf("[C]Ack %d Received\n",a.anum);
                break;
            } else {
                printf("[C]Ack not received....Resending Frame %d\n", f.fnum);
            }
        }
    }
    close(sock);
}

