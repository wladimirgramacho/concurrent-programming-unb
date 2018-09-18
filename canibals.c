/**************************************************************\
|*                                                            *|
|*   Course: Concurrent Programming - CIC/UnB                 *|
|*   Teacher: Eduardo Alchieri                                *|
|*   Author: Wladimir Ganzelevitch Mesquita Gramacho          *|
|*   Registration: 15/0048718                                 *|
|*                                                            *|
\**************************************************************/


#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

#define N_CANNIBALS 5
#define TRUE 1

pthread_mutex_t table;
pthread_cond_t cook;
pthread_cond_t cannibals;

int food = 0;

void eat_food(int id){
  printf("Cannibal %d: eating food.. om nom nom\n", id);
  sleep(3);
}

void * cannibalize(void * arg){
  int id = *((int *) arg);
  while(TRUE){
    pthread_mutex_lock(&table);
    while(!food){
      printf("Cannibal %d waiting food!!\n", id);
      pthread_cond_wait(&cannibals, &table);
    }
    food--;
    if(food == 0){
      pthread_cond_signal(&cook);
    }
    pthread_mutex_unlock(&table);
    eat_food(id);
  }
}

void cook_food(){
  printf("Cooker is making food! \\o/\n");
  sleep(5);
  food += 5;
}

void * cooker(void * arg){
  while(TRUE){
    pthread_mutex_lock(&table);
    while(food){
      printf("Cooker sleeping... zZzZ\n");
      pthread_cond_wait(&cook, &table);
    }
    cook_food();
    pthread_cond_broadcast(&cannibals);
    pthread_mutex_unlock(&table);
  }
}

void init_pthread(int i, pthread_t * thread, void * thread_function){
  int * id = (int *) malloc(sizeof(int));
  *id = i;
  pthread_create(thread, NULL, thread_function, (void *) (id));
}

int main(){
  int i;

  printf("Cooker makes food so cannibals won't eat him!\n");
  printf(".. |o| |o| |o| \t )o)\n\n\n");

  pthread_mutex_init(&table, NULL);
  pthread_cond_init(&cook, NULL);
  pthread_cond_init(&cannibals, NULL);
  pthread_t ca[N_CANNIBALS], co;

  for (i = 0; i < N_CANNIBALS ; i++) init_pthread(i, &ca[i], cannibalize);
  init_pthread(0, &co, cooker);

  pthread_join(ca[0],NULL);

  return 0;
}