#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "my_socket.h"
#include "logUtil.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        char msg[] = "Usage: ./connect remote-host port";
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }

    char *remote_host = argv[1];
    int   remote_port = strtol(argv[2], NULL, 0);

    int sockfd = tcp_socket();

    fprintfwt(stderr, "connect: trying connect() to %s port %d\n", remote_host, remote_port);

    if (connect_tcp(sockfd, remote_host, remote_port) < 0) {
        fprintfwt(stderr, "connect: connect to %s port %d failed\n", remote_host, remote_port);
        if (close(sockfd) < 0) {
            err(1, "close");
        }
        exit(1);
    }

    return 0;
}
