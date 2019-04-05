#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[5];

void* playandcount(void *arg)
{
	pthread_t id = pthread_self();
	if (pthread_equal(id, tid[0])) {
		system("mkdir /home/fms/Documents/FolderProses1");
		system("ps -aux | head > /home/fms/Documents/FolderProses1/SimpanProses1.txt");
		system("zip -mj /home/fms/Documents/FolderProses1/KompresProses1.zip /home/fms/Documents/FolderProses1/SimpanProses1.txt");
		sleep(15);
		system("unzip /home/fms/Documents/FolderProses1/KompresProses1.zip -d /home/fms/Documents/FolderProses1/");
	} else if (pthread_equal(id, tid[1])) {
		system("mkdir /home/fms/Documents/FolderProses2");
		system("ps -aux | head > /home/fms/Documents/FolderProses2/SimpanProses2.txt");
		system("zip -mj /home/fms/Documents/FolderProses2/KompresProses2.zip /home/fms/Documents/FolderProses2/SimpanProses2.txt");
		sleep(15);
		system("unzip /home/fms/Documents/FolderProses2/KompresProses2.zip -d /home/fms/Documents/FolderProses2/");
	}
	return NULL;
}

int main(void)
{
	for (int i = 0; i < 2; i++) {
		pthread_create(&(tid[i]), NULL, &playandcount, NULL);
	}
	for (int i = 0; i < 2; i++)
		pthread_join(tid[i],NULL);
	return 0;
}