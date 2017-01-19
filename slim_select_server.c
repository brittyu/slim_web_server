#include "slim_select.h"
#include "signal.h"

extern int sockfd;
extern server_context_st *slim_server_context;

int
main()
{
    slim_server_context = NULL;
    signal(SIGINT, handler_signal);
    sockfd = create_afinet_socket();
    struct sockaddr_in skaddr = create_afinet_socket_address();
    slim_server_init();
    set_socket_opt(sockfd);
    socket_bind(sockfd, skaddr);
    begin_select_server(sockfd);

    return 0;
}
