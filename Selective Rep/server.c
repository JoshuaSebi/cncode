#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define LOCALHOST "127.0.0.1"

typedef struct {
    int fnum;
    char data[30];
} Frame;

typedef struct {
    int anum;
} Ack;

int main() {
    int sock;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t addr_len = sizeof(clientaddr);
    Frame f;
    Ack a;
    int expected[10] = {0}; // buffer for received frames
    int drop3 = 0;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    bind(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("\n--- SELECTIVE REPEAT ARQ (SERVER) ---\n\n");

    int count = 1;
    while (count <=10) {
        int n = recvfrom(sock, &f, sizeof(f), 0, (struct sockaddr *)&clientaddr, &addr_len);
        if (n <= 0) continue;

        printf("[S] Frame %d received.\n", f.fnum);

        // Simulate loss of Frame 3
        if (f.fnum == 4 && drop3 == 0) {
            printf("[S] Frame %d lost (no ACK sent).\n", f.fnum);
            drop3 = 1;
            continue;
        }

        // Accept and ACK the frame
        expected[f.fnum] = 1;
        a.anum = f.fnum;
        sendto(sock, &a, sizeof(a), 0, (struct sockaddr *)&clientaddr, addr_len);
        printf("[S] ACK %d sent.\n", a.anum);
        count++;
        sleep(1);
    }

    printf("\n[S] All frames received successfully!\n");
    close(sock);
    return 0;
}
