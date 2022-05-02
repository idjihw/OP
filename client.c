#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE	1337

int main(void)
{
	int cnt = 0;
	int fd;
	int score = 100;
	char buf[BUF_SIZE];
	char pingstr[] = "pong\n";
	sem_t* sem;

	/*
	 * To-do : initialize for game
	 * get named semaphore, fifo, etc. 
	 */

	fd = open("pingpong.txt", O_RDWR|O_CREAT, S_IRWXU);
	
	sem_unlink("pingongsema");
	if((sem = sem_open("pingongsema", O_CREAT, 0644, 1)) == SEM_FAILED){
		perror("open");
		exit(1);
	}

	

	/*
	 * To-do : Round1 ping
	 * without [opponent] string
	 */



	for (cnt=0; cnt<5; cnt++)
	{
		sem_wait(sem);

		read(fd, buf, BUF_SIZE);

		printf("[opponent] %s", buf);
		printf("Your turn!\n");
		memset(buf, 0x00, BUF_SIZE);
		/*
		 * To-do : complete game process
		 */
		fgets(buf, BUF_SIZE, stdin);	
		
		if (strcmp(buf, pingstr))
		{
			printf("wrong! -20\n");
			score -= 20;
		}
		write(fd, buf, strlen(buf));

		sem_post(sem);
		sleep(1);
	}

	printf("Done! Your score : %d\n", score);

	close (fd);

	return 0;
}
