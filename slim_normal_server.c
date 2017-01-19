#include "extern.h"
#include "slim_normal.h"
#include "request.h"
#include "signal.h"

extern int sockfd;

int main()
{
    signal(SIGINT, handler_signal);

    sockfd = create_afinet_socket();

    struct sockaddr_in skaddr = create_afinet_socket_address();
    socket_bind(sockfd, skaddr);

    begin_normal_server(sockfd, skaddr);

    return 0;
}
