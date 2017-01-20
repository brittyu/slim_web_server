#include "slim_poll.h"
#include "signal.h"

extern int sockfd;

int
main()
{
    int listen_fd, conn_fd, sock_fd;
    listen_fd = create_afinet_socket();
    struct sockaddr_in skaddr = create_afinet_socket_address();
    set_socket_opt(listen_fd);
    socket_bind(listen_fd, skaddr);
    begin_poll_server(listen_fd);

    return 0;
}
