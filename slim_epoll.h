#ifndef _EPOLL
#define _EPOLL

#ifndef _EXTERN
#include "extern.h"
#endif

#ifndef _HTTP
#include "request.h"
#endif

#define SIZE 1024
#define FD_SIZE 1000
#define EPOLL_EVENTS 100

extern int create_afinet_socket();
extern struct sockaddr_in create_afinet_socket_address();
extern void socket_bind(int socket_fd, struct sockaddr_in skaddr);
extern void begin_epoll_server(int socket_fd);
extern void handle_events(int epoll_fd, struct epoll_event *events, int num, int listen_fd, char *buf);
extern void handle_accept(int epoll_fd, int listen_fd);
extern void do_write(int epoll_fd, char *buf);
extern void add_event(int epoll_fd, int fd, int state);
extern void delete_event(int epoll_fd, int fd, int state);
extern void modify_event(int epoll_fd, int fd, int state);
extern void do_read(int epoll_fd, int fd, char *buf);

#endif
