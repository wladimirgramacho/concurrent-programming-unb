/**************************************************************\
|*                                                            *|
|*   Course: Concurrent Programming - CIC/UnB                 *|
|*   Teacher: Eduardo Alchieri                                *|
|*   Author: Wladimir Ganzelevitch Mesquita Gramacho          *|
|*   Registration: 15/0048718                                 *|
|*                                                            *|
\**************************************************************/


// Produtor produz sempre que tem espaço no buffer
// Consumidor consome do buffer e dorme quando não tem nada no buffer

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"
#include "time.h"

#define N_MAKER 3
#define N_CONSUMER 6
#define BUFFER_SIZE 10
#define TRUE 1

pthread_mutex_t buffer_l;
pthread_cond_t maker_l;
pthread_cond_t consumer_l;

int stuff = 0;

void print_buffer() {
  int i;
  printf("BUFFER -> [");
  for (i = 1; i <= stuff; ++i){
    if(i == BUFFER_SIZE) printf("®");
    else printf("®|");
  }

  for (i = 1; i <= BUFFER_SIZE - stuff; ++i){
    if(i == BUFFER_SIZE - stuff) printf(" ");
    else printf(" |");
  }
  printf("]\n\n");
}

void make_stuff(int id){
  printf("Producer %d: making stuff.. plin plon tin\n", id);
  stuff += BUFFER_SIZE - stuff;
  print_buffer();
  sleep(rand() % 3);
}

void * make(void * arg){
  int id = *((int *) arg);
  while(TRUE){
    pthread_mutex_lock(&buffer_l);
    while(stuff == BUFFER_SIZE){
      printf("FULL BUFFER -> Producer %d having a break!!\n", id);
      pthread_cond_wait(&maker_l, &buffer_l);
    }
    make_stuff(id);
    pthread_cond_signal(&consumer_l);
    pthread_mutex_unlock(&buffer_l);
  }
}

void use_stuff(int id){
  printf("Consumer %d is using stuff! \\o/\n", id);
  stuff--;
  print_buffer();
  sleep(rand() % 2);
}

void * consume(void * arg){
  int id = *((int *) arg);
  while(TRUE){
    pthread_mutex_lock(&buffer_l);
    while(!stuff){
      printf("Consumer sleeping... zZzZ\n");
      pthread_cond_wait(&consumer_l, &buffer_l);
    }
    use_stuff(id);
    if(stuff < BUFFER_SIZE){
      pthread_cond_signal(&maker_l);
    }
    pthread_mutex_unlock(&buffer_l);
    sleep(rand() % 4);
  }
}

void init_pthread(int i, pthread_t * thread, void * thread_function){
  int * id = (int *) malloc(sizeof(int));
  *id = i;
  pthread_create(thread, NULL, thread_function, (void *) (id));
}

int main(){
  int i;

  printf("Makers make stuff. Consumers consume them!\n\n\n");
  print_buffer();

  pthread_mutex_init(&buffer_l, NULL);
  pthread_cond_init(&maker_l, NULL);
  pthread_cond_init(&consumer_l, NULL);
  pthread_t m[N_MAKER], c[N_CONSUMER];

  for (i = 0; i < N_MAKER ; i++) init_pthread(i, &m[i], make);
  for (i = 0; i < N_CONSUMER ; i++) init_pthread(i, &c[i], consume);

  pthread_join(m[0],NULL);

  return 0;
}