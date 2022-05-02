#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

#define LED_R	3
#define LED_Y	2
#define LED_G	0
#define SW_R	6
#define SW_Y	5
#define SW_G	4
#define SW_W	27

int a[5] = {0};
int b[5] = {0};
int b_index = 0;
sem_t* isok;

void start(void);
void init(void);
void off(void);
void fail(void);
void LEDR(void);
void LEDY(void);
void LEDG(void);
void LEDW(void);
void game(void);

int main(void){
    init();
    start();
    
    int status;
    pthread_t pt[4];
    
    sem_unlink("isok");
    if((isok = sem_open("isok", O_CREAT, 0644, 0)) == SEM_FAILED){
	perror("Sem failed");
	exit(1);
    }

    pthread_create(&pt[0], NULL, (void*)LEDR, NULL);
    pthread_create(&pt[1], NULL, (void*)LEDY, NULL);
    pthread_create(&pt[2], NULL, (void*)LEDG, NULL);
    pthread_create(&pt[3], NULL, (void*)LEDW, NULL);

    game();

    return 0;
}

void start(void){
	int i = 0;

	while(i < 2){
		digitalWrite(LED_R, 1);
		delay(250);
		digitalWrite(LED_R, 0);

		digitalWrite(LED_Y, 1);
		delay(250);
		digitalWrite(LED_Y, 0);

		digitalWrite(LED_G, 1);
		delay(250);
		digitalWrite(LED_G, 0);
		i++;
	}
}

void init(void){
	if(wiringPiSetup() == -1){
		puts("Setup Fail");
		exit(1);
	}
	pinMode(SW_R, INPUT);
	pinMode(SW_Y, INPUT);
	pinMode(SW_G, INPUT);
	pinMode(SW_W, INPUT);
	pinMode(LED_R, OUTPUT);
	pinMode(LED_Y, OUTPUT);
	pinMode(LED_G, OUTPUT);
	off();
}

void off(void){
	digitalWrite(LED_R, 0);
	digitalWrite(LED_Y, 0);
	digitalWrite(LED_G, 0);
}

void fail(void){
    	int i = 0;

	while(i < 3){
	    	delay(150);
		digitalWrite(LED_R, 1);
		digitalWrite(LED_Y, 1);
		digitalWrite(LED_G, 1);
		delay(150);
		off();
		
		i++;
	}
}

void game(void){
	sem_t ok;
	
	srand(time(NULL));
	int g = 0;
	int h;

	while(g < 5){
	    	h = g + 1;

		for(int i = 0; i < h; i++){
			a[i] = rand() % 3 + 1;
		}
		
		for(int i = 0; i < h; i++){
			if(a[i] == 1){
			    	delay(500);
				digitalWrite(LED_R, 1);
				delay(500);
				digitalWrite(LED_R, 0);
				
			}
			else if(a[i] == 2){
			    	delay(500);
				digitalWrite(LED_Y, 1);
				delay(500);
				digitalWrite(LED_Y, 0);
				
			}
			else if(a[i] == 3){
			    	delay(500);
				digitalWrite(LED_G, 1);
				delay(500);
				digitalWrite(LED_G, 0);
				
			}
			else
			    break;
			
		}
		
		sem_wait(isok);

		for(int i =0; i < 5; i++){
			if(a[i] == b[i])
			    continue;
			else{
			    fail();
			    exit(1);
			}	
		}
		
		for(int i = 0; i < 5; i++){
			b[i] = 0;
		}

		b_index = 0;

	    	g++;
	}
	start();
	exit(1);
}

void LEDR(void){
	while(1){
		if(digitalRead(SW_R) == 0){
			digitalWrite(LED_R, 1);
			delay(250);
			digitalWrite(LED_R, 0);
			b[b_index] = 1;
			b_index += 1;
	
		}
	
		sleep(1);
	}
		
		
	
	
}

void LEDY(void){
    	while(1){
		if(digitalRead(SW_Y) == 0){
			digitalWrite(LED_Y, 1);
			delay(250);
			digitalWrite(LED_Y, 0);
			b[b_index] = 2;
			b_index += 1;
		
		}
	
		sleep(1);
	}
    
}

void LEDG(void){
	
    	while(1){    	
		if(digitalRead(SW_G) == 0){
			digitalWrite(LED_G, 1);
			delay(250);
			digitalWrite(LED_G, 0);
			b[b_index] = 3;
			b_index += 1;
		
		}
	
		sleep(1);
	}
	
	
}

void LEDW(void){
    	

	while(1){
		if(digitalRead(SW_W) == 0){
			sem_post(isok);
			delay(500);
		}
		sleep(1);
	}
	
}
