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

#define MMAP1_NAME 	"io"
#define MMAP2_NAME 	"369GAME"


int main(void){
	
	int fd1;
	int fd2;
	int* p_mmap;
	int* result;
	int zero = 0;
	sem_t* sem;
	int pid = 0;
	int client_pid = 0;

	int client1, client2, client;
	
	puts("Please wait until server is ready...:");

	fd1 = open(MMAP1_NAME, O_RDWR|O_CREAT, 0664);	
	fd2 = open(MMAP2_NAME, O_RDWR|O_CREAT, 0664);

	write(fd1, &zero, sizeof(int));
	write(fd2, &zero, sizeof(int));

	p_mmap = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);
	result = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0);
	
	sem_unlink("client_sema");

	if((sem = sem_open("client_sema", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED){
		perror("open");
		exit(1);
	}

	puts("Done.\n");

	pid = getpid();
	printf("Server %d is ready for play\n", pid);
	
	sem_wait(sem);
	
	sleep(1);

	while(1){	// player1 
		sleep(1);
		sem_getvalue(sem, &client1);
		if(client1 == 1){
			printf("Client %d hi with slot 0\n",*(p_mmap));
			client1 = *(p_mmap);
			break;
		}
	}

	sem_wait(sem);

	while(1){	// player2
		sleep(1);		
		sem_getvalue(sem, &client2);
		if(client2 == 1){
			printf("Client %d hi with slot 1\n",*(p_mmap));
			client2 = *(p_mmap);
			break;
		}
	}
	
	*(p_mmap) = 0;
	*(result) = 0;
	
	sleep(5);
	
	for(int i = 1; ;i++){
			
		sem_wait(sem);

		if(*(result) != 0){
			printf("[%d]", *(result));
		}
		if(*(p_mmap) != 0){
			printf("%d\n", *(p_mmap));		
		}
			
		if(i%10==3 | i%10==6 | i%10==9){
			if(*(p_mmap) != -1){

				if(client1 == *(result)){
					printf("Player %d lose!\n", client1);
					kill(client1,SIGUSR2);
					kill(client2,SIGUSR1);
					printf("Client %d out\n", client1);
				}
				else{
					printf("Player %d lose!\n", client2);
					kill(client1,SIGUSR1);
					kill(client2,SIGUSR2);
					printf("Client %d out\n", client2);
				}
					
				exit(1);
			}
		}
		
		else{
			if(*(p_mmap) != i){
				
				if(client1 == *(result)){
					printf("Player %d lose!\n", client1);
					kill(client1,SIGUSR2);
					kill(client2,SIGUSR1);
					printf("Client %d out\n", client1);
				}
				else{
					printf("Player %d lose!\n", client2);
					kill(client1,SIGUSR1);
					kill(client2,SIGUSR2);
					printf("Client %d out\n", client2);
				}
				exit(1);
			}
		}

		client_pid = *(result);	
		
		*(result) = *(p_mmap); 
		*(p_mmap) = client_pid;
	
		sem_post(sem);
			
		sleep(1);
		
	}

}

