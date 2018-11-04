/**************************************************************\
|*                                                            *|
|*   Course: Concurrent Programming - CIC/UnB                 *|
|*   Teacher: Eduardo Alchieri                                *|
|*   Author: Wladimir Ganzelevitch Mesquita Gramacho          *|
|*   Registration: 15/0048718                                 *|
|*                                                            *|
\**************************************************************/

/**************************************************************\
|*                                                            *|
|*    Overcooked is a game where players share a kitchen so   *|
|*    they can bake various dishes. This program intends to   *|
|*    simulate the game using concurrent threads to make      *|
|*    dishes with more efficiency.                            *|
|*                                                            *|
\**************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "semaphore.h"
#include "unistd.h"
#include "time.h"

#define N_PLAYERS 4
#define CUTTING_BOARDS 2
#define TRUE 1
#define DELAY_TIME 0

void red()    { printf("\033[1;31m"); }
void green()  { printf("\033[1;32m"); }
void yellow() { printf("\033[1;33m"); }
void blue()   { printf("\033[1;34m"); }
void white()   { printf("\033[1;37m"); }
void reset()  { printf("\033[0m"); }

typedef void (*f)(void);
f color_funcs[N_PLAYERS] = {&red, &green, &yellow, &blue};

int available_ingredients = 0;
int players_points[N_PLAYERS] = {0};

pthread_mutex_t ingredients;
pthread_cond_t ingredient_ready;
sem_t chopping_table;
pthread_mutex_t frying_pan;

void * ingredient_treadmill(void * arg){
  while(TRUE){
    pthread_mutex_lock(&ingredients);
    available_ingredients++;
    reset();
    printf("[+1] -> Ingredientes disponíveis = %d\n", available_ingredients);
    pthread_cond_signal(&ingredient_ready);
    pthread_mutex_unlock(&ingredients);
    sleep((rand() % 3) + DELAY_TIME);
  }
}

void get_ingredient(int id){
  pthread_mutex_lock(&ingredients);
  while(!available_ingredients){
    color_funcs[id]();
    printf("Jogador %d esperando ingredientes\n", id);
    reset();
    pthread_cond_wait(&ingredient_ready, &ingredients);
  }
  available_ingredients--;
  pthread_mutex_unlock(&ingredients);
}

void chopping(int id){
  sem_wait(&chopping_table);
  color_funcs[id]();
  printf("Jogador %d cortando ingrediente\n", id);
  reset();
  sleep((rand() % 2) + DELAY_TIME);
}

void frying(int id){
  pthread_mutex_lock(&frying_pan);
  sem_post(&chopping_table);
  color_funcs[id]();
  printf("Jogador %d usando frigideira\n", id);
  reset();
  sleep((rand() % 5) + DELAY_TIME);
}

void deliver(int id){
  color_funcs[id]();
  printf("Jogador %d entregando prato! \\o/\n", id);
  reset();
  pthread_mutex_unlock(&frying_pan);
  players_points[id]++;
}

void print_ranking(){
  white();
  printf("\nRANKING: ");
  for (int i = 0; i < N_PLAYERS; ++i){
    color_funcs[i]();
    printf("J%d[%d]  ", i, players_points[i]);
    reset();
  }
  printf("\n\n");
  reset();
  sleep((rand() % 2) + DELAY_TIME);
}

void * play(void * arg){
  int id = *((int *) arg);
  while(TRUE){
    get_ingredient(id);
    chopping(id);
    frying(id);
    deliver(id);
    print_ranking();
  }
}

void init_pthread(int i, pthread_t * thread, void * thread_function){
  int * id = (int *) malloc(sizeof(int));
  *id = i;
  pthread_create(thread, NULL, thread_function, (void *) (id));
}


int main(){
  int i;
  pthread_t treadmill, players[N_PLAYERS];
  
  pthread_mutex_init(&ingredients, NULL);
  pthread_mutex_init(&frying_pan, NULL);
  pthread_cond_init(&ingredient_ready, NULL);
  sem_init(&chopping_table, 0, CUTTING_BOARDS);
  init_pthread(0, &treadmill, ingredient_treadmill);
  for (i = 0; i < N_PLAYERS ; i++) init_pthread(i, &players[i], play);

  pthread_join(treadmill,NULL);

  return 0;
}