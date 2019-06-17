#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

using namespace std;

int main()
{
    int sock;
    int snd_buf = 1024 * 2;
    int rcv_buf = 1024 * 2;
    int state;
    socklen_t len;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));
    if (state < 0) {
        cout << "setsockopt error" << endl;
        exit(-1);
    }

    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));
    if (state < 0) {
        cout << "setsockopt error " << endl;
        exit(-1);
    } 
    
    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state < 0) {
        cout << "RCVBUF get error" << endl;
        exit(-1);
    }
    cout << "rcv_buf : " << rcv_buf << endl;

    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state < 0) {
        cout << "SNDBUF get error" << endl;
        exit(-1);
    }
    cout << "snd_buf : " << snd_buf << endl;
    return 0;
}