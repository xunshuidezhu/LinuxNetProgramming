#include <assert.h>
#include <fcntl.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXFILE 65535
pthread_mutex_t mutex;

void handle_the_thread(void* arg)
{
    pthread_mutex_init(&mutex, NULL);
}

int main()
{
    pid_t pc;
    int fd, len;
    char msg[] = "DAEMON\n";
    len = strlen(msg);

    //step 1
    pc = fork();
    assert(pc > 0 || pc == 0);
    if (pc > 0) {
        exit(0);
    }

    //step 2
    setsid(); //create a new session for a process
    //之前parent和child运行在同一个session里,parent是会话（session）的领头进程,
    //parent进程作为会话的领头进程，如果exit结束执行的话，那么子进程会成为孤儿进程，并被init收养。
    //执行setsid()之后,child将重新获得一个新的会话(session)id。
    //这时parent退出之后,将不会影响到child了。

    //step 3
    chdir("/"); //= cd

    //step 4
    umask(0);
    // 第一步，我们首先写下目录具有全部权限的模式，即777 (所有用户都具有读、写和执行权限)。
    // 第二步，在下面一行按照umask值写下相应的位，在本例中是0 0 2。
    // 第三步，在接下来的一行中记下上面两行中没有匹配的位。这就是目录的缺省创建权限。
    // 稍加练习就能够记住这种方法。7 7 5
    // 第四步，对于文件来说，在创建时不能具有执行权限，只要拿掉相应的执行权限比特即可。6 6 4

    //step 5
    for (int i = 0; i < MAXFILE; ++i) {
        close(i);
    }
    while (1) {
        if ((fd = open("/tmp/daemon.log", O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) {
            perror("open error!");
            exit(1);
        }
        write(fd, msg, len + 1);
        close(fd);
        sleep(2);
    }
}
