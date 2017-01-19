#ifndef _SELECT
#define _SELECT

#ifndef _EXTERN
#include "extern.h"
#endif

#ifndef _HTTP
#include "request.h"
#endif

#define MAXLINE 1024
#define SIZE 10

typedef struct server_context_st
{
    int cli_cnt;
    int clifds[SIZE];
    fd_set all_fds;
    int max_fd;
} server_context_st;

server_context_st *slim_server_context;

extern int create_afinet_socket();
extern struct sockaddr_in create_afinet_socket_address();
extern void slim_server_init();
extern void set_socket_opt(int sockfd);
extern void socket_bind(int sockfd, struct sockaddr_in skaddr);
extern void begin_select_server(int sockfd);
extern void accept_client(int sockfd);
extern void recv_client(fd_set *read_fds);
extern void handle_client(int sockfd, char *);

#endif
