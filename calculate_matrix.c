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
#include <string.h> /* memset */
#include "unistd.h"
#include "time.h"

#define MATRIX_SIZE 10
#define TRUE 1

pthread_barrier_t barrier;

int matrix[MATRIX_SIZE][MATRIX_SIZE];
int sum_lines[MATRIX_SIZE], matrix_sum = 0;

void init_pthread(int i, pthread_t * thread, void * thread_function){
  int * id = (int *) malloc(sizeof(int));
  *id = i;
  pthread_create(thread, NULL, thread_function, (void *) (id));
}

int calculate_line(int id){
  int count = 0, i;
  for(i = 0; i < MATRIX_SIZE; i++) count += matrix[id][i];
  printf("Line %d calculated line - total value = %d\n", id, count);
  return count;
}

void * calculate(void * arg) {
  int id = *((int *) arg), i, line;
  
  line = calculate_line(id);
  sum_lines[id] = line;
  pthread_barrier_wait(&barrier);
  
  if(id == 0) for (i = 0; i < MATRIX_SIZE; ++i) matrix_sum += sum_lines[i];
}

int main(){
  int i;

  for(i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++) matrix[i/MATRIX_SIZE][i%MATRIX_SIZE] = 1;

  pthread_barrier_init(&barrier, NULL, MATRIX_SIZE);

  pthread_t m[MATRIX_SIZE];

  for (i = 0; i < MATRIX_SIZE ; i++) init_pthread(i, &m[i], calculate);


  pthread_join(m[0],NULL);

  printf("Total matrix sum = %d\n", matrix_sum);
  return 0;
}