#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("time out ...");
    }
    alarm(1);
}

int main()
{
    int i;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    
    alarm(1);
    for (i = 0; i < 3; i++) {
        puts("wait...");
        sleep(2);
    }
    return 0;
}