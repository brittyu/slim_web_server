#include "normal.h"

int
create_afinet_socket()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    return sockfd;
}

struct sockaddr_in
create_afinet_socket_address()
{
    struct sockaddr_in skaddr;

    skaddr.sin_family = AF_INET;
    skaddr.sin_port = htons(PORT);
    skaddr.sin_addr.s_addr = inet_addr(SERV);

    return skaddr;
}

void
socket_bind(int sockfd, struct sockaddr_in skaddr)
{
    if (bind(sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    if (listen(sockfd, QUEUE) == -1) {
        perror("Listen error");
        exit(1);
    }
}

void
begin_normal_server(int sockfd, struct sockaddr_in skaddr)
{
    int count = 0;
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
}
