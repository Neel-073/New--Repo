#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    FILE* fp = fopen(filename, "rb");

    fseek(fp, 0, SEEK_END);
    uint64_t filesize = ftell(fp);
    rewind(fp);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    inet_pton(AF_INET, "192.168.28.194", &server.sin_addr);

    connect(sockfd, (struct sockaddr*)&server, sizeof(server));

    uint32_t filename_len = strlen(filename);
    uint32_t net_filename_len = htonl(filename_len);
    uint64_t net_filesize = htobe64(filesize);

    send(sockfd, &net_filename_len, sizeof(net_filename_len), 0);
    send(sockfd, filename, filename_len, 0);
    send(sockfd, &net_filesize, sizeof(net_filesize), 0);

    char buffer[BUFFER_SIZE];
    size_t n;

    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(sockfd, buffer, n, 0);
    }

    fclose(fp);
    close(sockfd);
}