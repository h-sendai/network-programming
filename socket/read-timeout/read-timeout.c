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
#include "set_timer.h"

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
    int debug  = 0;
    int n_loop = 10;

    int c;
    while ( (c = getopt(argc, argv, "dn:")) != -1) {
        switch (c) {
            case 'd':
                debug = 1;
                break;
            case 'n':
                n_loop = strtol(optarg, NULL, 0);
                break;
            default:
                break;
        }
    }
    argc -= optind;
    argv += optind;

    if (argc != 3) {
        char msg[] = "Usage: connect-timeout [-d (debug)] [-n n_loop] remote port read_timeout_sec\n"
                     "read_timeout_sec may be decimal point number\n";
        fprintf(stderr, "%s\n", msg);
        exit(1);
    }

    char *remote          = argv[0];
    int  port             = strtol(argv[1], NULL, 0);
    struct timeval read_timeout;
    conv_str2timeval(argv[2], &read_timeout);

    if (debug) {
        fprintf(stderr, "read_timeout.tv_sec: %ld, read_timeout.tv_usec: %ld\n",
            read_timeout.tv_sec, read_timeout.tv_usec);
    }

    prctl(PR_SET_TIMERSLACK, 1);
    int sockfd = tcp_socket();

    if (connect_tcp(sockfd, remote, port) < 0) {
        errx(1, "connect_tcp");
    }

    if (set_so_rcvtimeout(sockfd, read_timeout.tv_sec, read_timeout.tv_usec) < 0) {
        errx(1, "set_so_rcvtimeout");
    }

    set_start_tv();
    log_et(stderr, "program start\n");

    for (int i = 0; i < n_loop; ++i) {
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
