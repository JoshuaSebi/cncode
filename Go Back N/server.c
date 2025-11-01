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
    struct sockaddr_in serveraddr,clientaddr;
    struct timeval tv={4,0};
    Ack a;
    Frame f;

    sock=socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(PORT);
    serveraddr.sin_addr.s_addr=LOCALHOST;
    socklen_t clen=sizeof(clientaddr);

    bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    int drop3=0;
    int base=1;
    while(base<=7){
        int n=recvfrom(sock,&f,sizeof(f), 0,(struct sockaddr*)&clientaddr, &clen);
        if(n<=0) continue;
        printf("[S] Frame %d recvd\n",f.fnum);

        if(f.fnum==3 && drop3==0){
            printf("[S] Frame %d dropped Intentionally\n",f.fnum);
            drop3=1;
            continue;
        }

        if(f.fnum==base){
            a.anum=f.fnum;
            printf("[S] Ack sent for Frame %d\n",f.fnum);
            sendto(sock, &a, sizeof(a),0,(struct sockaddr*)&clientaddr,clen);
            base++;
        } else {
            a.anum=base-1;
            printf("[S] Ack sent for Frame %d\n",f.fnum);
            sendto(sock, &a, sizeof(a),0,(struct sockaddr*)&clientaddr,clen);
        }
    }
    close(sock);
}