#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sys/wait.h>

#define BUFSIZE 30

using namespace std;

int main()
{
    int fds1[2], fds2[2];
    //pipe1
    pipe(fds1);
    //pipe2
    pipe(fds2);
    char str1[] = "message 1";
    char str2[] = "message 2";
    char buf[BUFSIZE];
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        //child proc
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, BUFSIZE);
        cout << buf << endl;
    } else {
        //parent proc
        read(fds1[0], buf, BUFSIZE);
        write(fds2[1], str2, sizeof(str2));
        sleep(1);
        cout << buf << endl;
        
        waitpid(-1, 0, 0);
    }
    return 0;
}