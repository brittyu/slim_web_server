#ifndef _POLL
#define _POLL

#ifndef _EXTERN
#include "extern.h"
#endif

#ifndef _HTTP
#include "request.h"
#endif

#define OPEN_MAX 1000
#define MAX_LINE 1024
#define INFTIM -1

extern int create_afinet_socket();
extern struct sockaddr_in create_afinet_socket_address();
extern void socket_bind(int socket_fd, struct sockaddr_in skaddr);
extern void begin_poll_server(int socket_fd);
extern void handle_connection(struct pollfd *conn_fds, int num);

#endif
