#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#define MMAP1_NAME	"io"
#define MMAP2_NAME 	"369GAME"


void end(int); 

int main(void){

	int fd1;
	int fd2;
	int* p_mmap;
	int* result;
	int server;
	int game;
	int pid = 0;
	char c;
	struct sigaction act;
	sem_t* sem;	

	fd1 = open(MMAP1_NAME, O_RDWR, 0664);	
	fd2 = open(MMAP2_NAME, O_RDWR, 0664);
	

	p_mmap = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);
	result = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0);

	if((sem = sem_open("client_sema", O_EXCL)) == SEM_FAILED){
		perror("sem_open failed.");
		puts("An error is accured, tell admin.");
		exit(1);
	}

	sem_post(sem);

	pid = getpid();
	*(p_mmap) = pid;

	puts("waiting for server..\n");
	puts("please wait for 3 seconds for synchronization");
	sleep(3);
	

	puts("=============3 6 9 GAME=============");
	puts("\t Hello player");
	puts("\t Welcome to 369GAME");
	puts("====================================");
	
	
	puts("Please Enter any key to continue");
	c = getc(stdin);
	
	
	
	for(int i = 1; ;i++){
		sleep(1);
		sem_wait(sem);	
		act.sa_handler = end;
		
		sigaction(SIGUSR1, &act, NULL);
		sigaction(SIGUSR2, &act, NULL);
		sigaction(SIGINT, &act, NULL);
		
		if(*(p_mmap) != 0){
			printf("[%d]", *(p_mmap));	
		}
		if(*(result) != 0){
			printf("%d\n", *(result));	
		}	
			
		printf("input num : ");
		scanf("%d", p_mmap);
		printf("[%d]", pid);
		printf("%d\n", *(p_mmap));
		*(result) = pid;

		sem_post(sem);
		
		sleep(1);
		
	}
	

}

void end(int sig){
	switch(sig){
		case SIGUSR1:
			puts("You Win!");
			exit(1);
		case SIGUSR2:
			puts("You Lose!");
			exit(1);
	}
}

