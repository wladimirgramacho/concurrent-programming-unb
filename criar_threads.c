#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "unistd.h"

#define N 2

void * pthread_func(void * arg){
    int id = *((int *) arg); 
    printf("Criou um pthread com id = %d \n",id);
    pthread_exit(0);    
}

int main() {
  pthread_t a[N];
  int i;
  int * id;
  for (i = 0; i < N ; i++) {
    id = (int *) malloc(sizeof(int));
    *id = i;
    pthread_create(&a[i], NULL, pthread_func, (void *) (id));
  }
  for (i = 0; i < N ; i++) {
    pthread_join(a[i],NULL);
  }  
  printf("TERMINANDO\n");
  return 0;
}

















