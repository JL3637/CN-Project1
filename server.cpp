#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* host = "0.0.0.0";
#define port 7000
#define Maxlen 4096

int main(){
    int host_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (host_sock_fd == -1) {
        perror("Socket creation error");
        exit(1);
    }
    struct sockaddr_in host_addr, client_addr;
    int status;

    host_addr.sin_family = AF_INET;
    inet_aton(host, &host_addr.sin_addr);
    host_addr.sin_port = htons(port);

    status = bind(host_sock_fd, (struct sockaddr *)&host_addr, sizeof(host_addr));
    if (status == -1) {
        perror("Binding error");
        exit(1);
    }
    printf("server start at: %s:%d\n", inet_ntoa(host_addr.sin_addr), port);

    status = listen(host_sock_fd, 5);
    if (status == -1) {
        perror("Listening error");
        exit(1);
    }
    printf("wait for connection...\n");

    socklen_t client_addrlen = sizeof(client_addr);

    char buf_in[Maxlen] = {0}, buf_out[Maxlen] = {0};
    while (1) {
        int client_sock_fd = accept(host_sock_fd, (struct sockaddr *)&client_addr, &client_addrlen);
        printf("Connected by %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        while (1) {
            int buf_len = recv(client_sock_fd, buf_in, sizeof(buf_in), 0);
            if (buf_len <= 0) {
                close(client_sock_fd);
                printf("Client closed connection.\n");
                break;
            }
            buf_in[buf_len] = '\0';
            printf("Server recv: %s\n", buf_in);

            sprintf(buf_out, "%s", buf_in);
            send(client_sock_fd, buf_out, strlen(buf_out), 0);
        }
    }
    close(host_sock_fd);

    return 0;
}