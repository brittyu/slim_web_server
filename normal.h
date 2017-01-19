#ifndef _NORMAL
#define _NORMAL

#include "extern.h"

#ifndef _HTTP
#include "request.h"
#endif

extern int create_afinet_socket();
extern struct sockaddr_in create_afinet_socket_address();
extern void socket_bind(int sockfd, struct sockaddr_in skaddr);
extern void begin_normal_server(int sockfd, struct sockaddr_in skaddr);

#endif
