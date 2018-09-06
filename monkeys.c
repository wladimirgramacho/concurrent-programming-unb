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

#define N_MONKEYS_A 5
#define N_MONKEYS_B 5
#define A 'A'
#define B 'B'

pthread_mutex_t rope;
pthread_mutex_t turn;
pthread_mutex_t lock_a;
pthread_mutex_t lock_b;

int a_monkeys_on_rope = 0;
int b_monkeys_on_rope = 0;

void use_rope(char c, int id){
  int monkeys = c == A ? a_monkeys_on_rope : b_monkeys_on_rope;
  printf("Monkey %d from %c using rope. We have %d monkeys on rope\n", id, c, monkeys);
  sleep(1);
}

void leaving_rope(char c){
  printf("\n¯\\_(ツ)_/¯ Monkeys from %c leaving rope ¯\\_(ツ)_/¯\n\n", c);
  sleep(3);
}

void * monkey_a(void * arg){
  int id = *((int *) arg);
  while(1){
    pthread_mutex_lock(&turn);
    pthread_mutex_lock(&lock_a);
    if(a_monkeys_on_rope == 0){
      pthread_mutex_lock(&rope);
    }
    pthread_mutex_unlock(&lock_a);
    a_monkeys_on_rope++;
    pthread_mutex_unlock(&turn);

    use_rope(A, id);

    pthread_mutex_lock(&lock_a);
    a_monkeys_on_rope--;
    if(a_monkeys_on_rope == 0){
      leaving_rope(A);
      pthread_mutex_unlock(&rope);
    }
    pthread_mutex_unlock(&lock_a);
  }
}

void * monkey_b(void * arg){
  int id = *((int *) arg);
  while(1){
    pthread_mutex_lock(&turn);
    pthread_mutex_lock(&lock_b);
    if(b_monkeys_on_rope == 0){
      pthread_mutex_lock(&rope);
    }
    pthread_mutex_unlock(&lock_b);
    b_monkeys_on_rope++;
    pthread_mutex_unlock(&turn);

    use_rope(B, id);

    pthread_mutex_lock(&lock_b);
    b_monkeys_on_rope--;
    if(b_monkeys_on_rope == 0){
      leaving_rope(B);
      pthread_mutex_unlock(&rope);
    }
    pthread_mutex_unlock(&lock_b);
  }
}

void init_pthread(int i, pthread_t * thread, void * thread_function){
  int * id = (int *) malloc(sizeof(int));
  *id = i;
  pthread_create(thread, NULL, thread_function, (void *) (id));
}

int main(){
  int i;

  pthread_mutex_init(&rope, NULL);
  pthread_mutex_init(&turn, NULL);
  pthread_t a[N_MONKEYS_A], b[N_MONKEYS_B];

  for (i = 0; i < N_MONKEYS_A ; i++) init_pthread(i, &a[i], monkey_a);
  for (i = 0; i < N_MONKEYS_B ; i++) init_pthread(i, &b[i], monkey_b);

  pthread_join(a[0],NULL);

  return 0;
}