#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFFERSIZE 30

using namespace std;

void read_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, 0);
}

int main(int argc, char* argv[])
{
    //the pipe[fds[2]]
    int fds[2];
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = read_childproc;
    char str1[] = "the message 1";
    char str2[] = "the message 2";
    char buf[BUFFERSIZE];
    pid_t pid;

	pipe(fds);
    pid = fork();

    if (pid == 0) {
        //child proc write the message
        write(fds[1], str1, sizeof(str1));
        sleep(2);
        //child proc read the message
        read(fds[0], buf, BUFFERSIZE);
        cout << "child proc read the message " << buf << endl;
    } else if (pid > 0) {
        //father proc read the message
        read(fds[0], buf, BUFFERSIZE);
        cout << "parent proc read the message " << buf << endl;
        //father proc write the message
        write(fds[1], str2, sizeof(str2));
        sleep(5);
    } else {
		cout << "for error" << endl;
		exit(1);
	}

    return 0;
}
