#include <stdio.h>
#include <stdlib.h>

static void exit_handler1(),exit_handler2();

int main(){
	if(atexit(exit_handler1) != 0)
		perror("exit_hadnler1 등록할 수 없습니다.\n");
	if(atexit(exit_handler2) != 0)
		perror("exit_hadnler2 등록할 수 없습니다.\n");
	
	printf("main() 끝\n");

	exit(0);
}

static void exit_handler1(){
	printf("첫 번째 exit 처리기\n");
}
static void exit_handler2(){
	printf("두 번째 exit 처리기\n");
}



