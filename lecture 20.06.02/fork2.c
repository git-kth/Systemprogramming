#include <stdio.h>
#include <unistd.h>

int main(){
	int pid;

	pid = fork();
	if(pid == 0){
		printf("[child] : Hello, World PID = %d\n",getpid());
		printf("[child] : Hello, World PID = %d\n",getpid());
		printf("[child] : Hello, World PID = %d\n",getpid());
		printf("[child] : Hello, World PID = %d\n",getpid());
		printf("[child] : Hello, World PID = %d\n",getpid());
		printf("[child] : Hello, World PID = %d\n",getpid());

	}
	else{
		printf("[Parent] : Hello, World PID = %d\n",getpid());
		printf("[Parent] : Hello, World PID = %d\n",getpid());
		printf("[Parent] : Hello, World PID = %d\n",getpid());
		printf("[Parent] : Hello, World PID = %d\n",getpid());
		printf("[Parent] : Hello, World PID = %d\n",getpid());
		printf("[Parent] : Hello, World PID = %d\n",getpid());
	}
}
