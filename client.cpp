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
    int client_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_sock_fd == -1){
        perror("Socket creation error");
        exit(1);
    }

    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    inet_aton(host, &client_addr.sin_addr);
    client_addr.sin_port = htons(port);

    int status = connect(client_sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    if (status == -1) {
        perror("Connection error");
        exit(1);
    }

    char buf_in[Maxlen] = {0}, buf_out[Maxlen] = {0};
    while (1) {
        printf("please input your message: ");
        fgets(buf_out, sizeof(buf_out), stdin);
        printf("Client send: %s\n", buf_out);
        send(client_sock_fd, buf_out, strlen(buf_out), 0);

        int buf_len = recv(client_sock_fd, buf_in, sizeof(buf_in), 0);
        if (buf_len <= 0) {
            close(client_sock_fd);
            printf("Server closed connection.\n");
            break;
        }
        buf_in[buf_len] = '\0';
        printf("Server recv: %s\n", buf_in);
    }

    return 0;
}