#include "slim_select.h"

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
handle_client(int sockfd, char *buf)
{
    printf("Recv buff is: %s\n", buf);
    write(sockfd, buf, strlen(buf) + 1);
}

void
slim_server_init()
{
    slim_server_context = (server_context_st *)malloc(sizeof(server_context_st));
    if (slim_server_context == NULL) {
        perror("slim server init error");
        exit(1);
    }

    memset(slim_server_context, 0, sizeof(slim_server_context));

    for (int i = 0; i < SIZE; i++) {
        slim_server_context->clifds[i] = -1;
    }
}

void
set_socket_opt(int sockfd)
{
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("set socket fail");
        exit(1);
    }
}

void 
socket_bind(int sockfd, struct sockaddr_in skaddr)
{
    if (bind(sockfd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    if (listen(sockfd, QUEUE) == -1) {
        perror("listen error");
        exit(1);
    }
}

void
accept_client(int sockfd)
{
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    cliaddrlen = sizeof(cliaddr);
    int clifd = -1;

ACCEPT:
    clifd = accept(sockfd, (struct sockaddr_in *)&cliaddr, &cliaddrlen);
    
    if (clifd == -1) {
        if (errno == EINTR) {
            goto ACCEPT;
        } else {
            perror("Accept fail");
            exit(1);
        }
    }

    int i = 0;
    for (i = 0; i < SIZE; i++) {
        if (slim_server_context->clifds[i] < 0) {
            slim_server_context->clifds[i] = clifd;
            slim_server_context->cli_cnt++;
            break;
        }
    }

    if (i == SIZE) {
        perror("Too many clients");
        exit(1);
    }
}

void
recv_client(fd_set *read_fds)
{
    int i = 0, n = 0;
    int clifd;
    char buf[MAXLINE] = {0};
    for (i = 0; i <= slim_server_context->cli_cnt; i++) {
        clifd = slim_server_context->clifds[i];
        if (clifd < 0) {
            continue;
        }

        if (FD_ISSET(clifd, read_fds)) {
            n = read(clifd, buf, MAXLINE);
            if (n <= 0) {
                FD_CLR(clifd, &slim_server_context->all_fds);
                close(clifd);
                slim_server_context->clifds[i] = -1;
                continue;
            }
        }
    }
}

void
begin_select_server(int sockfd)
{
    int clifd = -1;
    int retval = 0;
    fd_set *read_fds = &slim_server_context->all_fds;

    struct timeval slim_time_val;
    int i = 0;

    while (1) {
        FD_ZERO(read_fds);
        FD_SET(sockfd, read_fds);

        slim_server_context->max_fd = sockfd;

        slim_time_val.tv_sec = 30;
        slim_time_val.tv_usec = 0;

        for (i = 0; i < slim_server_context->cli_cnt; i++) {
            clifd = slim_server_context->clifds[i];
            
            if (clifd != -1) {
                FD_SET(clifd, read_fds);
            }

            slim_server_context->max_fd = (clifd > slim_server_context->max_fd) ? \
                                          clifd : slim_server_context->max_fd;
        }

        retval = select(slim_server_context->max_fd + 1, read_fds, NULL, NULL,\
                &slim_time_val);

        if (retval == -1) {
            perror("select error");
            exit(1);
        }

        if (retval == 0) {
            perror("select timeout");
            continue;
        }

        if (FD_ISSET(sockfd, read_fds)) {
            accept_client(sockfd);
        } else {
            recv_client(read_fds);
        }
    }
}
