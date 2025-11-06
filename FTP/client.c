// FILE CLIENT USING TCP (newline-based filename separator)

#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

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
    struct sockaddr_in serveraddr;
    FILE *fp;
    char buffer[MAXSIZE], filename[MAXSIZE];

    // Step 1: Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    errorcheck(sock, "Socket Created.");

    // Step 2: Setup server details
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = LOCALHOST;

    // Step 3: Connect
    int c = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    errorcheck(c, "Connection Established");

    // Step 4: Get and send filename (include newline!)
    fgets(filename,MAXSIZE,stdin);
    filename[strcspn(filename,"\n")]='\0';
    // Step 5: Open and send file data
    fp = fopen(filename, "r");
    if (!fp) {
        perror("FILE NOT FOUND!!!");
        close(sock);
        exit(1);
    }

    printf("\nSending File Content to Server....\n");
    int r;
    r=fread(buffer,1,MAXSIZE,fp);
    buffer[r]='\n';
    send(sock, buffer, r, 0);
    printf("%s\n",buffer);


    // Step 6: Receive confirmation from server
    memset(buffer,0,MAXSIZE);
    int x = recv(sock, buffer, MAXSIZE, 0);
    if (x > 0) {
        buffer[r] = '\0';
        printf("Server: %s\n", buffer);
    }

    fclose(fp);
    close(sock);
    return 0;
}
