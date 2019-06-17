#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(void)
{
    int fd;
    int result;
    int status = 0;
    char* cResult;
    int readLen;
    char buf[10];
    pid_t pid;
    fd = open("./demo5.bat", O_RDWR);
    pid = fork();
    if (pid == 0) {
        execlp("./demo.bash", NULL);

    } else {
        readLen = read(fd, buf, 10);
        result = atoi(&buf[0]);
        cout << result << endl;
        wait(&status);
        
    }

    return 0;
}