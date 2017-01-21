#include "slim_epoll.h"

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
    if (bind(socket_fd, (struct sockaddr_in *)&skaddr, sizeof(skaddr)) == -1) {
        perror("Bind error");
        exit(1);
    }

    if (listen(socket_fd, QUEUE) == -1) {
        perror("Listen error");
        exit(1);
    }
}

void
begin_epoll_server(int listen_fd)
{
    int ret;
    struct epoll_event events[EPOLL_EVENTS];
    char buf[SIZE];
    memset(buf, 0, SIZE);

    int epoll_fd = epoll_create(FD_SIZE);

    add_event(epoll_fd, listen_fd, EPOLLIN);

    while (1) {
        ret = epoll_wait(epoll_fd, events, EPOLL_EVENTS, -1);
        handle_events(epoll_fd, events, ret, listen_fd, buf);
    }

    close(epoll_fd);
}

void
handle_events(int epoll_fd, struct epoll_event *events, int num, int listen_fd, char *buf)
{
    int i;
    int fd;
    for (i = 0; i < num; i++) {
        fd = events[i].data.fd;
        if ((fd == listen_fd) && (events[i].events & EPOLLIN)) {
            handle_accept(epoll_fd, listen_fd);
        } else if (events[i].events & EPOLLIN) {
            do_read(epoll_fd, fd, buf);
        } else if (events[i].events & EPOLLOUT) {
            do_write(epoll_fd, fd, buf);
        }
    }
}

void
handle_accept(int epoll_fd, int listen_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    int client_fd = accept(listen_fd, (struct sockaddr_in *)&client_addr, &client_addr_len);
    
    if (client_fd == -1) {
        perror("Accept error");
        exit(1);
    } else {
        add_event(epoll_fd, client_fd, EPOLLIN);
    }
}

void
add_event(int epoll_fd, int fd, int state)
{
    struct epoll_event new_event;

    new_event.events = state;
    new_event.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &new_event);
}

void
delete_event(int epoll_fd, int fd, int state)
{
    struct epoll_event delete_event;

    delete_event.events = state;
    delete_event.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &delete_event);
}

void
modify_event(int epoll_fd, int fd, int state)
{
    struct epoll_event event;

    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
}

void
do_read(int epoll_fd, int fd, char *buf)
{
    int status;

    status = read(fd, buf, SIZE);
    if (status == -1) {
        perror("Read error");
        close(fd);
        delete_event(epoll_fd, fd, EPOLLIN);
    } else if (status == 0) {
        close(fd);
        delete_event(epoll_fd, fd, EPOLLIN);
    } else {
        modify_event(epoll_fd, fd, EPOLLIN);
    }
}

void
do_write(int epoll_fd, int fd, char *buf)
{
    int nwrite;
    nwrite = write(fd, buf, strlen(buf));

    if (nwrite == -1) {
        perror("Write error");
        close(fd);
        delete_event(epoll_fd, fd, EPOLLOUT);
    } else {
        modify_event(epoll_fd, fd, EPOLLIN);
    }

    memset(buf, 0, SIZE);
}
