#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8000

pthread_t tid[2]; //0 = display, 1 = terima
int *stock;
int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
 
void* routine(void *arg)
{
    pthread_t id = pthread_self();
    if (pthread_equal(id, tid[0])) {
        while (1) {
            printf("Stock: %d\n", *stock);
            sleep(5);
        }
    } else {
        while (1) {
            valread = read( new_socket , buffer, 1024); 
            (*stock)++;
        }
    }

    return NULL;
}

int main(int argc, char const *argv[]) {
    key_t key = 1234;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);
    *stock = 0;
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    pthread_create(&(tid[0]),NULL,&routine,NULL);
    pthread_create(&(tid[1]),NULL,&routine,NULL);
    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}