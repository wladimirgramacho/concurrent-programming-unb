/**************************************************************\
|*                                                            *|
|*   Matéria: Programação Concorrente                         *|
|*   Professor: Eduardo Alchieri                              *|
|*   Nome: Wladimir Ganzelevitch Mesquita Gramacho            *|
|*   Matrícula: 15/0048718                                    *|
|*                                                            *|
\**************************************************************/


#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

#define NUM_LEITORES 5
#define NUM_ESCRITORES 5

pthread_mutex_t banco;
pthread_mutex_t lock_leitores;
pthread_mutex_t turno;

int readers = 0;

void le_banco(int id){
  printf("Leitor %d: lendo banco de dados\n", id);
  sleep(1);
}

void * leitores(void * arg){
  int id = *((int *) arg);
  while(1){
    pthread_mutex_lock(&turno);
    pthread_mutex_lock(&lock_leitores);
    if(readers == 0){
      pthread_mutex_lock(&banco);
    }
    readers++;
    pthread_mutex_unlock(&lock_leitores);
    pthread_mutex_unlock(&turno);

    le_banco(id);

    pthread_mutex_lock(&lock_leitores);
    readers--;
    if(readers == 0){
      pthread_mutex_unlock(&banco);
    }
    pthread_mutex_unlock(&lock_leitores);
  }
}

void escreve_banco(int id){
  printf("Escritor %d: escrevendo no banco de dados\n", id);
  sleep(1);
}

void * escritores(void * arg){
  int id = *((int *) arg);
  while(1) {
    pthread_mutex_lock(&turno);
    pthread_mutex_lock(&banco);
    pthread_mutex_unlock(&turno);
    escreve_banco(id);
    pthread_mutex_unlock(&banco);
  }
}

int main(){
  pthread_mutex_init(&banco, NULL);
  pthread_mutex_init(&lock_leitores, NULL);
  pthread_mutex_init(&turno, NULL);

  pthread_t l[NUM_LEITORES], e[NUM_ESCRITORES];
  int i;
  int * id;
  for (i = 0; i < NUM_LEITORES ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&l[i], NULL, leitores, (void *) (id));
  }
  for (i = 0; i < NUM_ESCRITORES ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&e[i], NULL, escritores, (void *) (id));
  }

  for (i = 0; i < NUM_LEITORES; i++) {
    pthread_join(l[i],NULL);
  }
  for (i = 0; i < NUM_ESCRITORES; i++) {
    pthread_join(e[i],NULL);
  }


  printf("TERMINANDO\n");
  return 0;
}