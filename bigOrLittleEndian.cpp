/**********************************
 * test the PC is big endian / little endian
 * use the union
 * 
 * *******************************/
#include <sys/utsname.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <stdio.h>


using namespace std;

int main()
{
    union bigOrLittleEndian
    {
        /* data */
        short inum;
        char c;
    } un;
    utsname uts;
    un.inum = 0x1234;
    if (uname(&uts) < 0){
        cout << " error " <<  flush;
    }
    
    cout << uts.machine << " " << uts.sysname << " " << uts.release << endl;
    if (un.c == 0x12 )
        cout << " big " << endl;
    else if (un.c == 0x34)
        cout << " little " << endl;
    else{
        cout << " error " << endl;
    }
}
