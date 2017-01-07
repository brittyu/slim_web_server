#include "request.h"

HTTP_CONTENT_TYPE http_content_type[] = {
    {"html", "text/html"},
    {"gif", "image/gif"},
    {"jpeg", "image/jpeg"}
};

char *http_res_tmpl = "HTTP/1.1 200 OK\r\n"
    "Server: Brittyu's Server V1.0\r\n"
    "Accept-Ranges: bytes\r\n"
    "Content-Length: %d\r\n"
    "Connection: close\r\n"
    "Content-Type: %s\r\n\r\n";

void http_send(int sock_client, char *content)
{
    char HTTP_HEADER[BUFF_SIZE], HTTP_INFO[BUFF_SIZE];
    int len = strlen(content);
    sprintf(HTTP_HEADER, http_res_tmpl, len, "text/html");
    len = sprintf(HTTP_INFO, "%s%s", HTTP_HEADER, content);

    send(sock_client, HTTP_INFO, len, 0);
}
