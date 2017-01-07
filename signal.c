#include <stdio.h>
#include "extern.h"
#include "signal.h"

extern int sockfd;

void handler_signal(int sign)
{
    fputs("\nSIGNAL INTERRUPT \nBye Brittyu! \nSAFE EXIT\n", stdout);
    close(sockfd);
    exit(0);
}
