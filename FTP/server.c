// FILE SERVER USING TCP (newline-based filename separator + vowel count)

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 256
#define PORT 2319
#define LOCALHOST inet_addr("127.0.0.1")

void errorcheck(int x, const char *msg) {
    if (x < 0) {
        perror("Error Occurred");
        exit(1);
    } else {
        printf("%s\n", msg);
    }
}

int main() {
    int sock;
    struct sockaddr_in serveraddr, clientaddr;
    FILE *fp;
    char filename[MAXSIZE], buffer[MAXSIZE];
    int vowel_count = 0;

    // Step 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    errorcheck(sock, "Socket Created");

    // Step 2: Setup server address
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = LOCALHOST;

    // Step 3: Bind
    int b = bind(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    errorcheck(b, "Binding Done");

    // Step 4: Listen
    int l = listen(sock, 5);
    errorcheck(l, "Listening...");

    while (1) {
        socklen_t clen = sizeof(clientaddr);
        int newsock = accept(sock, (struct sockaddr*)&clientaddr, &clen);
        errorcheck(newsock, "Connection Established!!!");

        // Step 5: Receive filename (until newline)
        

        fp = fopen("abc.txt", "a");
        if (!fp) {
            perror("File open failed");
            close(newsock);
            continue;
        }

        // Step 6: Receive file content and count vowels
        int w=recv(newsock, buffer, MAXSIZE, 0);
            fwrite(buffer, 1, w, fp);
            buffer[w] = '\0';  // for safe string printing
            printf("===========\n%s\n===========\n", buffer);

            // Count vowels
            for (int j = 0; j < w; j++) {
                char c = tolower(buffer[j]);
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                    vowel_count++;
                }
                }
            memset(buffer, 0, MAXSIZE);

        printf("File written successfully.\n");
        printf("Total vowels in file: %d\n", vowel_count);

        char msg[] = "Data Written Successfully\n";
        send(newsock, msg, strlen(msg), 0);

        fclose(fp);
        close(newsock);
        vowel_count = 0; // reset for next file
    }

    close(sock);
    return 0;
}
