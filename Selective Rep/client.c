#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define LOCALHOST "127.0.0.1"
#define TOTAL_FRAMES 10
#define WIN_SIZE 3

typedef struct {
    int fnum;
    char data[30];
} Frame;

typedef struct {
    int anum;
} Ack;

int main() {
    int sock;
    struct sockaddr_in serveraddr;
    socklen_t addr_len = sizeof(serveraddr);
    Frame f;
    Ack a;
    struct timeval timeout = {3, 0};

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr(LOCALHOST);

    int base = 1;
    int next = 1;
    int acked[TOTAL_FRAMES + 1] = {0};

    printf("\n--- SELECTIVE REPEAT ARQ (CLIENT) ---\n\n");

    while (base <= TOTAL_FRAMES) {
        // Send frames in the window
        while (next < base + WIN_SIZE && next <= TOTAL_FRAMES) {
            f.fnum = next;
            sprintf(f.data, "Frame_%d", next);
            printf("[C] Sending Frame %d...\n", next);
            sendto(sock, &f, sizeof(f), 0, (struct sockaddr *)&serveraddr, addr_len);
            next++;
        }

        int n = recvfrom(sock, &a, sizeof(a), 0, (struct sockaddr *)&serveraddr, &addr_len);

        if (n > 0) {
            printf("[C] ACK %d received.\n", a.anum);
            acked[a.anum] = 1;
        } else {
            printf("[C] Timeout! Resending missing frames in window...\n");
            for (int i = base; i < next; i++) {
                if (!acked[i]) {
                    f.fnum = i;
                    sprintf(f.data, "Frame_%d", i);
                    printf("[C] Resending Frame %d...\n", i);
                    sendto(sock, &f, sizeof(f), 0, (struct sockaddr *)&serveraddr, addr_len);
                }
            }
        }

        while (base <= TOTAL_FRAMES && acked[base]) {
            base++;
        }

        sleep(1);
    }

    printf("\n[C] All frames sent and acknowledged successfully!\n");
    close(sock);
    return 0;
}
