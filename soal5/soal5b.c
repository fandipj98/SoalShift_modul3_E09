#include<stdio.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<termios.h>

pthread_t tid[1];
int *shop_stock;

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

void* shop(void *arg)
{
        pthread_t id = pthread_self();
        while (1) {
                if (pthread_equal(id, tid[0])) {
                        system("clear");
                        printf("Shop\nFood stock : %d\nChoices\n1. Restock\n2. Exit\n", shop_stock[0]);
                        sleep(1);
                }
        }
        return NULL;
}


void main()
{
        key_t key = 1234;
        int input;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        shop_stock = shmat(shmid, NULL, 0);

        pthread_create(&(tid[0]), NULL, &shop, NULL);
        
        while (1) {
                input = mygetch() -'0';
                
                if(input==1){
                        shop_stock[0] = shop_stock[0] + 1;
                }
                else if(input==2){
                        puts("You Exit The Shop");
                        exit(0);
                }
        }
        //shmdt(shop_stock);
        //shmctl(shmid, IPC_RMID, NULL);
}
