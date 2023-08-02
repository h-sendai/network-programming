# シグナル

[man 2 signal](https://man7.org/linux/man-pages/man2/signal.2.html)

```
NAME
      signal - ANSI C signal handling
SYNOPSIS
      #include <signal.h>
      typedef void (*sighandler_t)(int);
      sighandler_t signal(int signum, sighandler_t handler);
DESCRIPTION
      The  behavior  of  signal() varies across UNIX versions, and has also varied historically
      across different versions of Linux.  Avoid its use: use sigaction(2) instead.  See Porta‐
      bility below.
```

[man 2 sigaction](https://man7.org/linux/man-pages/man2/sigaction.2.html)

```
NAME
       sigaction, rt_sigaction - examine and change a signal action
SYNOPSIS
       #include <signal.h>
       int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);
DESCRIPTION
       The sigaction() system call is used to change the action taken by a process on receipt of
       a specific signal.  (See signal(7) for an overview of signals.)

       signum specifies the signal and can be any valid signal except SIGKILL and SIGSTOP.

       If act is non-NULL, the new action for signal signum is installed from act.  If oldact is
       non-NULL, the previous action is saved in oldact.

       The sigaction structure is defined as something like:

           struct sigaction {
               void     (*sa_handler)(int);
               void     (*sa_sigaction)(int, siginfo_t *, void *);
               sigset_t   sa_mask;
               int        sa_flags;
               void     (*sa_restorer)(void);
           };
```

[man 7 signal (overview of signals)](https://man7.org/linux/man-pages/man7/signal.7.html)
