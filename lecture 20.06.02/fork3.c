#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
		int pid1, pid2;
		pid1 = fork();
		if (pid1 == 0) {
				printf("[Child 1] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 1] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 1] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 1] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 1] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 1] : Hello, world ! pid=%d\n", getpid());
				exit(0);
		}else{
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());	
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
		}
		pid2 = fork();
		if (pid2 == 0) {
				printf("[Child 2] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 2] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 2] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 2] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 2] : Hello, world ! pid=%d\n", getpid());
				printf("[Child 2] : Hello, world ! pid=%d\n", getpid());
				exit(0);
		}else{
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());	
			printf("[Parent] : Hello, world ! pid=%d\n", getpid());
		
		}
}
