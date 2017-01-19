#ifndef _HTTP
#define _HTTP

#define PORT 8787
#define SERV "0.0.0.0"
#define QUEUE 20
#define BUFF_SIZE 1024

typedef struct doc_type
{
    char *key;
    char *value;
} HTTP_CONTENT_TYPE;

extern void http_send(int sock, char *content);

#endif
