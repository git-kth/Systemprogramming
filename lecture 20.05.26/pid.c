#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int pid;
	printf("나의 프로세스 번호: [%d]\n",getpid());
	printf("내 부모 프로세스 번호: [%d]\n",getppid());
}
