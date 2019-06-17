#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

void read_childproc(int sig)
{
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        cout << "remove id " << id << endl;
        cout << "child send : " << WEXITSTATUS(status) << endl;
    }
}

int main()
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (pid == 0) {
        puts("the child process");
        sleep(10);
        return 12;
    } else {
        cout << "child id" << pid << endl;
        pid = fork();
        if (pid == 0) {
            puts("the child process");
            sleep(10);
            exit(24);
        } else {
            int i;
            cout << "child proc id : " << pid << endl;
            for (i = 0; i < 5; i++) {
                cout << "wait ..." << endl;
                sleep(5);
            }
        }
    }
    return 0;
}