#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main()
{
    int status;
    pid_t pid = fork();

    if (pid == 0) {
        return 3;
    } else {
        cout << " child pid : " << pid << endl;
        pid = fork();
        if (pid == 0) {
            exit(7);
        } else {
            cout << "child pid : " << pid << endl;
            wait(&status);
            if (WIFEXITED(status)) 
                cout << "child one return " << WEXITSTATUS(status) << endl;
            wait(&status);
            if (WIFEXITED(status)) 
                cout << "child two return " <<  WEXITSTATUS(status) << endl;
            sleep(30); 
        }

    }
}