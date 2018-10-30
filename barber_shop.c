/**************************************************************\
|*                                                            *|
|*   Course: Concurrent Programming - CIC/UnB                 *|
|*   Teacher: Eduardo Alchieri                                *|
|*   Author: Wladimir Ganzelevitch Mesquita Gramacho          *|
|*   Registration: 15/0048718                                 *|
|*                                                            *|
\**************************************************************/


// Problema do barbeiro preguiçoso: clientes aparecem na barbearia.
// Se não houver clientes, barbeiro dorme. Se não houver vagas para 
// esperar barbeiro, cliente vai embora.

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"
#include "unistd.h"
#include "time.h"

#define N_CLIENTS 7
#define BARBER_SHOP_SIZE 5
#define TRUE 1

sem_t barber_chair;
sem_t clients_queue;
sem_t barber_on_chair;
sem_t client_on_chair;

int clients = 0;


void cut_hair(int id){
  printf("\t CUT CUT CUT\n", id);
  sleep(rand() % 3);
}

void * barber(void * arg){
  int id = *((int *) arg);
  while(TRUE){
    printf("Barber is sleeping zZzZ\n");
    sem_wait(&barber_on_chair);
    cut_hair(id);
    sem_post(&client_on_chair);
  }
}


void * client(void * arg){
  int id = *((int *) arg);
  while(TRUE){
    if(sem_trywait(&clients_queue) == 0) {
      printf("Client %d waiting on queue...\n", id);
      sem_wait(&barber_chair);
      sem_post(&clients_queue);
      printf("Client %d ready for barber on chair...\n", id);
      sem_post(&barber_on_chair);
      sem_wait(&client_on_chair);
      printf("\\o/ Client %d going home!\n", id);
      sem_post(&barber_chair);
    }
    else {
      printf("Queue is too big!! Client %d going home..\n", id);
    }
    sleep(rand() % 5);
  }
}

void init_pthread(int i, pthread_t * thread, void * thread_function){
  int * id = (int *) malloc(sizeof(int));
  *id = i;
  pthread_create(thread, NULL, thread_function, (void *) (id));
}

int main(){
  int i;

  sem_init(&clients_queue, 0, BARBER_SHOP_SIZE);
  sem_init(&client_on_chair, 0, 0);
  sem_init(&barber_chair, 0, 1);
  sem_init(&barber_on_chair, 0, 0);

  pthread_t b, c[N_CLIENTS];

  init_pthread(0, &b, barber);
  for (i = 0; i < N_CLIENTS ; i++) init_pthread(i, &c[i], client);

  pthread_join(c[0],NULL);

  return 0;
}