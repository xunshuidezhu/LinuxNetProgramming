#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int total_ticket_num = 20;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* sell_ticket(void* arg) {
    for (int i = 0; i < 20; ++i) {
        pthread_mutex_lock(&mutex);
        if (total_ticket_num > 0) {
            sleep(1);
            cout << "sell tickets: " << 20 - total_ticket_num + 1 << endl;
            --total_ticket_num; 
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

int main ()
{
    int iRet;
    pthread_mutex_init(&mutex, NULL);
    pthread_t tids[4];
    int i = 0;
    for (i = 0; i < 4; ++i) {
        iRet = pthread_create(&tids[i], NULL, &sell_ticket, NULL);
        if (iRet) {
            cout << "pthread_create error: " << iRet << endl;
            return iRet; 
        }
    }
    sleep(30);
    
    void* retval;
    for (i = 0; i < 4; ++i) {
        iRet = pthread_join(tids[i], &retval);
        if (iRet) {
            cout << tids[i] << " join error: " << iRet << endl;
            return iRet;
        }
        cout << (long*)retval << endl;
        cout << "thread" << i << " is end ! " << endl;
    }
    
    return 0;
}