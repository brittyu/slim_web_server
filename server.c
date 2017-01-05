#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>

#define PORT 8787
#define SERV "0.0.0.0"
#define QUEUE 20
#define BUFF_SIZE 1024

typedef struct doc_type
{
    char *key;
    char *value;
} HTTP_CONTENT_TYPE;

HTTP_CONTENT_TYPE http_content_type[] = {
    {"html", "text/html"},
    {"gif", "image/gif"},
    {"jpeg", "image/jpeg"}
};

int sockfd;
char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
    "Server: Cleey's Server V1.0\r\n"
    "Accept-Ranges: bytes\r\n"
    "Content-Length: %d\r\n"
    "Connection: close\r\n"
    "Content-Type: %s\r\n\r\n";

void handler_signal(int sign);
void http_send(int sock, char *content);

int main()
{
    signal(SIGINT, handler_signal);

    int count = 0;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in skaddr;
    skaddr.sin_family = AF_INET;
    skaddr.sin_port = htons(PORT);
    skaddr.sin_addr.s_addr = inet_addr(SERV);

    if (bind(sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1) {
        perror("bind error");
        exit(1);
    }

    if (listen(sockfd, QUEUE) == -1) {
        perror("listen error");
        exit(1);
    }

    char buff[BUFF_SIZE];
    struct sockaddr_in claddr;
    socklen_t length = sizeof(claddr);

    while (1) {
        int sock_client = accept(sockfd, (struct sockaddr *)&claddr, &length);
        printf("%d\n", ++count);

        if (sock_client < 0) {
            perror("accept error");
            exit(1);
        }

        memset(buff, 0, sizeof(buff));
        int len = recv(sock_client, buff, sizeof(buff), 0);
        fputs(buff, stdout);
        http_send(sock_client, "hello world!");
        close(sock_client);
    }

    fputs("bye bye", stdout);
    close(sockfd);

    return 0;
}

void http_send(int sock_client, char *content)
{
    char HTTP_HEADER[BUFF_SIZE], HTTP_INFO[BUFF_SIZE];
    int len = strlen(content);
    sprintf(HTTP_HEADER, http_res_tmpl, len, "text/html");
    len = sprintf(HTTP_INFO, "%s%s", HTTP_HEADER, content);

    send(sock_client, HTTP_INFO, len, 0);
}

void handler_signal(int sign)
{
    fputs("\nSIGNAL INTERRUPT \nBye Brittyu! \nSAFE EXIT\n", stdout);
    close(sockfd);
    exit(0);
}
