#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
    int status;
    pid_t pid = fork();
    
    if (pid == 0) {
        sleep(15);
        return 24;
    } else {
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            puts("sleep 3sec");
        }
        if (WIFEXITED(status)) {
            cout << "child send " << WEXITSTATUS(status) << endl;
        }
    }
    return 0;
}