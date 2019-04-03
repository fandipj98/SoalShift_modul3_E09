#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[3]; //0 = agmal, 1 = iraj, 2 = status
int status[2]; // 0 = status agmal, 1 = status iraj
int inputflag, counter1, counter2, flag1, flag2;

void* agmalandiraj(void *arg)
{
	pthread_t id = pthread_self();
	while (1) {
		if (pthread_equal(id, tid[0])) {
			if(inputflag==2){
				//printf("%d %d %d %d %d\n",flag1,flag2,counter1,counter2,inputflag);
				if(flag1==1){
					status[0]+=15;
					counter1++;
					if(counter1==3){
						flag2=0;
					}
					if (status[0] >= 100) {
						puts("Agmal Terbangun,mereka bangun pagi dan berolahraga");
						exit(0);
					}
				}
				inputflag = 0;
			}else{
				if(counter2>=3){
					sleep(10);
					puts("Agmal Ayo Bangun enable again");
					counter2=counter2-3;
					flag1=1;
					inputflag = 0;
				}
			}

		} else if (pthread_equal(id, tid[1])) {
			if(inputflag==3){
				//printf("%d %d %d %d %d\n",flag1,flag2,counter1,counter2,inputflag);
				if(flag2==1){
					status[1]-=20;
					counter2++;
					if(counter2==3){
						flag1=0;
					}
					if (status[1] <= 0) {
						puts("Iraj ikut tidur, dan bangun kesiangan bersama Agmal");
						exit(0);
					}
				}
				inputflag = 0;
			}else{
				if(counter1>=3){
					sleep(10);
					puts("Iraj Ayo Tidur enable again");
					counter1=counter1-3;
					flag2=1;
					inputflag = 0;
				}
			}

		} else if (pthread_equal(id, tid[2])){
			if(inputflag==1){
				inputflag = 0;
				printf("Agmal WakeUp_Status: %d\nIraj Spirit_Status: %d\n", status[0], status[1]);
			}
		}
	}
	return NULL;
}



int main()
{
	char inputString[50];
	status[0] = 0;
	status[1] = 100;
	counter1 = 0;
	counter2 = 0;
	flag1 = 1;
	flag2 = 1;
	inputflag = 0;

	for (int i = 0; i < 3; i++) {
		int err = pthread_create(&(tid[i]), NULL, &agmalandiraj, NULL);
	}

	while (1) {		
		
		scanf(" %[^\t\n]s",inputString);
		
		//printf("%s\n",inputString);
		if (strcmp(inputString, "All Status") == 0) {

			inputflag = 1;

		} else if (strcmp(inputString, "Agmal Ayo Bangun") == 0){
			
			inputflag = 2;
			if(counter2>=3){
				puts("Agmal Ayo Bangun disabled 10 s");
			}

		} else if (strcmp(inputString, "Iraj Ayo Tidur") == 0){
			
			inputflag = 3;
			if(counter1>=3){
				puts("Fitur Iraj Ayo Tidur disabled 10 s");
			}

		}

	}
	exit(0);
	return 0;
}