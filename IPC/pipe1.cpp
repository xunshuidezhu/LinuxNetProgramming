#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFERSIZE 30

void foo()
{
}


int main()
{
	int fds[2];
	char str[] = "who are you" ;
	char buf[BUFFERSIZE];
	pid_t pid;
	foo();
	pipe(fds);
	pid = fork();
	if (pid == 0) {
		write(fds[1], str, sizeof(str));
	} else {
		read(fds[0], buf, BUFFERSIZE);
		puts(buf);
	}

}	
