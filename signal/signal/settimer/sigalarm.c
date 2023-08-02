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

#include "my_signal.h"
#include "set_timer.h"

//volatile sig_atomic_t got_alarm = 0;
int got_alarm = 0;

void sig_alarm(int signo)
{
    got_alarm = 1;

    return;
}

int main(int argc, char *argv[])
{
    my_signal(SIGALRM, sig_alarm);
    set_timer(1, 0, 1, 0);

    struct timeval start, now, prev, elapsed, delta;
    gettimeofday(&start, NULL);
    now = start;
    prev = now;

    for ( ; ; ) {
        /* FIXME: excessive cpu consumption */
        /* shoud use pause() or sigwait() */
        // pause();
        // usleep(1);
        if (got_alarm) {
            got_alarm = 0;
            gettimeofday(&now, NULL);
            timersub(&now, &prev,  &delta);
            timersub(&now, &start, &elapsed);
            printf("%ld.%06ld %ld.%06ld got_alarm\n",
                elapsed.tv_sec, elapsed.tv_usec,
                delta.tv_sec, delta.tv_usec
            );
            prev = now;
        }
    }

    return 0;
}
