#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <bits/signum.h>
#include <sys/epoll.h>

using namespace std;

void timeout(int sig) {
    if (sig == SIGALRM) 
        puts("time out");
    alarm(1); 
}
void keycontrol(int sig) {
    if (sig = SIGINT) {
        puts("CTRL+C pressed");
    }
}
void func(int sig) {
    if (sig == SIGALRM) {
        cout << "wait error " << endl;
    }
    alarm(1);
}
int main()
{
    int i;
    // signal(SIGALRM, timeout);
    signal(SIGALRM, func);
    signal(SIGINT, keycontrol);
    alarm(1);
    for (i = 0; i < 3; i++) {
        puts("wait...");
        sleep(3);
    }
    return 0;
}