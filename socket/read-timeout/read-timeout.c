#include <sys/prctl.h>
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
#include "timespecop.h"
#include "log_et.h"

int random_sleep(int sec)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    srandom(tv.tv_usec);

    int x = random();
    double y = (double) x  / (double) RAND_MAX;
    int usec = (int) sec*1000000.0*y;
    // fprintf(stderr, "usec: %d\n", usec);

    usleep(usec);

    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        char msg[] = "Usage: connect-timeout remote port read_timeout_sec";
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }

    char *remote          = argv[1];
    int  port             = strtol(argv[2], NULL, 0);
    int  read_timeout_sec = strtol(argv[3], NULL, 0);

    prctl(PR_SET_TIMERSLACK, 1);
    int sockfd = tcp_socket();

    if (connect_tcp(sockfd, remote, port) < 0) {
        errx(1, "connect_tcp");
    }

    if (set_so_rcvtimeout(sockfd, read_timeout_sec, 0) < 0) {
        errx(1, "set_so_rcvtimeout");
    }

    set_start_tv();
    log_et(stderr, "program start\n");

    for (int i = 0; i < 10; ++i) {
        char buf[1024];
        int n = read(sockfd, buf, sizeof(buf));
        if (n < 0) {
            if (errno == EAGAIN) {
                log_et(stderr, "read timeout\n");
            }
            else {
                err(1, "read");
            }
        }
    }
    return 0;
}
