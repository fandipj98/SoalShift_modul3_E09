#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<termios.h>

pthread_t tid[5]; //0=hunger, 1=hygiene, 2=mandi, 3=regen, 4=game
int hunger_stat, hygiene_stat, health_stat, monster_health, food_stock, *shop_stock; 
int cooldown, flag, input, mandi_flag;
char nama[50];

int mygetch ( void ) 
{
  int ch;
  struct termios oldt, newt;
  
  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );
  
  return ch;
}

void* hunger(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[0])) {
			if(flag==1){	
				sleep(10);
				if(flag==1){
					hunger_stat-=5;
					if(hunger_stat<=0){
						system("clear");
						printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath is ready\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock);
						puts("You died because of starvation");
						exit(0);
					}
				}
			}
		}
	}
	return NULL;
}

void* hygiene(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[1])) {
			if(flag==1){
				sleep(30);
				if(flag==1){
					hygiene_stat-=10;
					if(hygiene_stat<=0){
						system("clear");
						printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath is ready\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock);
						puts("You died because of dirty");
						exit(0);
					}
				}
			}
		}
	}
	return NULL;
}

void* mandi(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[2])) {
			if(mandi_flag==1){
				if(hygiene_stat<=100 && cooldown==0){	
					cooldown=20;
					hygiene_stat+=15;
					if(hygiene_stat>=100){
						hygiene_stat=100;
					}
				}
				mandi_flag=0;
			}
			while(cooldown>0){
				sleep(1);
				cooldown--;	
			}
		}
	}
	return NULL;
}

void* regen(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[3])) {
			if(flag==1){	
				sleep(10);
				if(health_stat<300 && flag==1){	
					health_stat+=5;
					if(health_stat>=300){
						health_stat=300;
					}
				}
			}
		}
	}
	return NULL;
}

void* game(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[4])) {
			if(flag==1){
				if(cooldown==0){
					system("clear");
					printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath is ready\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock);
					sleep(1);
				}
				else{
					system("clear");
					printf("Standby Mode\nMonster's Name: %s\nHealth : %d\nHunger : %d\nHygiene : %d\nFood Left : %d\nBath will be ready in %ds\nChoices\n1. Eat\n2. Bath\n3. Battle\n4. Shop\n5. Exit\n", nama, health_stat, hunger_stat, hygiene_stat, food_stock, cooldown);
					sleep(1);
				}
			}
			else if(flag==2){
				monster_health=100;
				while(1){
					sleep(1);
					system("clear");
					printf("Battle Mode\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\nChoices\n1. Attack\n2. Run\n", nama, health_stat, monster_health);
					input = mygetch() - '0';
					if(input==1){
						monster_health-=20;
						sleep(1);
						system("clear");
						printf("Battle Mode: your's turn\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\nChoices\n1. Attack\n2. Run\n", nama, health_stat, monster_health);
						if(monster_health<=0){
							puts("You win!");
							flag=1;
							break;
						}
						sleep(1);
						health_stat-=20;
						if(health_stat<=0){
							health_stat=0;
							system("clear");
							printf("Battle Mode\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\n", nama, health_stat, monster_health);
							
							puts("You died because killed");
							exit(0);
						}
						system("clear");
						printf("Battle Mode: enemy's turn\nMonster's Name: %s\nMonster's Health : %d\nEnemy's Health : %d\n", nama, health_stat, monster_health);
					}
					else if(input==2){
						puts("You ran away!");
						flag=1;
						break;
					}
				}
			}
			else if(flag==3){
				system("clear");	
				printf("Shop Mode\nShop food stock : %d\nYour food stock : %d\nChoices\n1. Buy\n2. Back\n", shop_stock[0], food_stock);
				sleep(2);
			}
		}
	}
	return NULL;
}



int main()
{
	key_t key = 1234;

	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    	shop_stock = shmat(shmid, NULL, 0);

	printf("Input Your Monster's Name: ");
	scanf("%s", nama);

	hunger_stat=200;
	hygiene_stat=100;
	health_stat=300;
	monster_health=100;
	food_stock=0;
	flag=1;
	cooldown=0;
	mandi_flag=0;

	pthread_create(&(tid[0]), NULL, &hunger, NULL);
	pthread_create(&(tid[1]), NULL, &hygiene, NULL);
	pthread_create(&(tid[2]), NULL, &mandi, NULL);
	pthread_create(&(tid[3]), NULL, &regen, NULL);
	pthread_create(&(tid[4]), NULL, &game, NULL);

	while (1) {		
		if(flag==1){
			input = mygetch() - '0';
			if(input==1){
				if(food_stock>0){
					if(hunger_stat<=200){	
						hunger_stat+=15;
						food_stock--;
						if(hunger_stat>=200){
							hunger_stat=200;
						}
					}
				}
				else{
					puts("Your Food Stock is Empty");
					sleep(1);
				}
			}
			else if(input==2 && cooldown==0){
				mandi_flag=1;
			}
			else if(input==3){
				flag=2;
			}
			else if(input==4){
				flag=3;
			}
			else if(input==5){
				puts("You Exit The Game");
				exit(0);
			}
		}
		else if(flag==3){
			input = mygetch() - '0';
			if(input==1){
				if(shop_stock[0]>0){
					food_stock++;
					shop_stock[0]--;	
				}
				else{
					puts("The Shop is Out of Stock");
				}
			}
			else if(input==2){
				flag=1;
			}
		}
	}
	exit(0);
	return 0;
}
