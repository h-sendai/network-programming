#include "my_signal.h"

Sigfunc *my_signal(int signo, Sigfunc *func)
{
    struct sigaction    act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
/* The reason for making a special case of SIGALRM is that the purpose
   of generating this signal is normally to place a timeout on an I/O
   operation, as we will show in Section 14.2, in which case, we want
   the blocked system call to be interrupted by the signal
   (Stevens, Fenner, Rudoff "UNIX Network Programming Section 5.8 (p. 131) */
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;   /* SunOS 4.x */
#endif
    } else {
#ifdef  SA_RESTART
        act.sa_flags |= SA_RESTART;     /* SVR4, 44BSD */
#endif
    }
    if (sigaction(signo, &act, &oact) < 0)
        return(SIG_ERR);
    return(oact.sa_handler);
}
