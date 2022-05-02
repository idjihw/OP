#include <semaphore.h>
#include <stdio.h>
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
	char pingstr[] = "ping\n";
	sem_t* sem;

	/*
	 * To-do : initialize for game
	 * get named semaphore, fifo, etc. 
	 */

	mkfifo("pingpong.txt", 0666);		// fifo 파일을 생성해줍니다
	fd = open("pingpong.txt", O_RDWR|O_CREAT, S_IRWXU);	// 파일 오픈

	sem_unlink("pingongsema");
	if((sem = sem_open("pingongsema", O_CREAT, 0644, 1)) == SEM_FAILED){	// Named semaphore
		perror("open");
		exit(1);
	}

	

	/*
	 * To-do : Round1 ping
	 * without [opponent] string
	 */

	printf("Your turn!\n");
	memset(buf, 0x00, BUF_SIZE);
	fgets(buf, BUF_SIZE, stdin);		// 먼저 입력을 받아줍니다
	
	if (strcmp(buf, pingstr)){		// ping이 아니라면 호출
		printf("wrong! -20\n");
		score -= 20;
	}

	write(fd, buf, strlen(buf));		// 파일에 작성

	sem_post(sem);			// 카운트 값 1 증가
	sleep(1);

	for (cnt=1; cnt<5; cnt++)
	{
		sem_wait(sem);		// 카운트 1 감소시키고 0이 됐으니 대기

		read(fd, buf, BUF_SIZE);	// 파일을 읽어 buf에 저장

		printf("[opponent] %s", buf);	// buf 출력
		printf("Your turn!\n");
		memset(buf, 0x00, BUF_SIZE);
		/*
		 * To-do : complete game process
		 */
		fgets(buf, BUF_SIZE, stdin);	// 입력
		
		if (strcmp(buf, pingstr))
		{
			printf("wrong! -20\n");
			score -= 20;
		}

		write(fd, buf, strlen(buf));	// 파일에 작성

		sem_post(sem);		// 카운트 1 증가
		sleep(1);
	}

	printf("Done! Your score : %d\n", score);

	close (fd);

	return 0;
}
