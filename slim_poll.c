#include "slim_poll.h"

int
create_afinet_socket()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    return socket_fd;
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
set_socket_opt(int socket_fd)
{
    int reuse = 1;
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("Set socket error");
        exit(1);
    }
}

void
socket_bind(int socket_fd, struct sockaddr_in skaddr)
{
    if (bind(socket_fd, (struct sockaddr *)&skaddr, sizeof(skaddr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    if (listen(socket_fd, QUEUE) == -1) {
        perror("Listen error");
        exit(1);
    }
}

void
begin_poll_server(int listen_fd)
{
    int conn_fd, socket_fd;
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len;
    struct pollfd cli_fds[OPEN_MAX];

    int maxi;
    int i;
    int nready;

    cli_fds[0].fd = listen_fd;
    cli_fds[0].events = POLLIN;

    for (i = 1; i < OPEN_MAX; i++) {
        cli_fds[i].fd = -1;
    }

    maxi = 0;

    while (1) {
        nready = poll(cli_fds, maxi + 1, INFTIM);

        if (nready == -1) {
            perror("Poll error");
            exit(1);
        }

        if (cli_fds[0].revents & POLLIN) {
            cli_addr_len = sizeof(cli_addr);
            if ((conn_fd = accept(listen_fd, (struct sockaddr_in *)&cli_addr, &cli_addr_len)) == -1) {
                if (errno == EINTR) {
                    continue;
                } else {
                    perror("Accept error");
                    exit(1);
                }
            }

            for (i = 1; i < OPEN_MAX; i++) {
                if (cli_fds[i].fd < 0) {
                    cli_fds[i].fd = conn_fd;
                    break;
                }
            }

            if (i == OPEN_MAX) {
                fprintf(stderr, "Too many clients.\n");
                exit(1);
            }

            cli_fds[i].events = POLLIN;

            maxi = (i > maxi ? i : maxi);

            if (--nready <= 0) {
                continue;
            }
        }

        handle_connection(cli_fds, maxi);
    }
}

void
handle_connection(struct pollfd *conn_fds, int num)
{
    int i, n;
    char buf[MAX_LINE];
    memset(buf, 0, MAX_LINE);

    for (i = 1; i <= num; i++) {
        if (conn_fds[i].fd < 0) {
            continue;
        }

        if (conn_fds[i].revents & POLLIN) {
            n = read(conn_fds[i].fd, buf, MAX_LINE);

            if (n == 0) {
                close(conn_fds[i].fd);
                conn_fds[i].fd = -1;
                continue;
            }
        }

        http_send(conn_fds[i].fd, "Hello World");
    }
}
