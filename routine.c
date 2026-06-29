#include "codexion.h"
#include <pthread.h>

bool wait_for_simulation_to_start(t_coder *coder);
static void compiling(t_coder *coder, t_queue *queue) ;
static void debuging(t_coder *coder);
static void refactoring(t_coder *coder);



void *routine_all_the_coders(void *arg) {
  t_coder *coder;
  coder = (t_coder *)arg;
  wait_for_simulation_to_start(coder);
  while (1)
  {
    compiling(coder, coder->queue);
    debuging(coder);
    refactoring(coder);
    // break;
  }

  return NULL;
}
static void refactoring(t_coder *coder)
{
  if (coder->coder_state == REFACTORING)
  {
    pthread_mutex_lock(coder->print_mutex);
    printf("coder %d is refactoring\n", coder->coder_id);
    coder->coder_state = WAITING;
    pthread_mutex_unlock(coder->print_mutex);
    drop_both_dongles(coder);
  }
}

static void debuging(t_coder *coder)
{
  if (coder->coder_state == DEBUGING)
  {
    pthread_mutex_lock(coder->print_mutex);
    printf("coder %d is debuging\n", coder->coder_id);
    coder->coder_state = REFACTORING;
    pthread_mutex_unlock(coder->print_mutex);
  }
}
static void compiling(t_coder *coder, t_queue *queue)
{
  if (coder->coder_state == COMPILING)
  {
    hold_both_dongles(coder) ;
    printf("coder is compiling\n");
    pthread_mutex_lock(coder->print_mutex);
    printf("coder %d is compiling\n", coder->coder_id);
    coder->coder_state = DEBUGING;
    pthread_mutex_unlock(coder->print_mutex);
  }
}

bool wait_for_simulation_to_start(t_coder *coder) {
  pthread_mutex_lock(&coder->ready_coders_counter_m_c->mutex);
  (*coder->ready_coders_counter)++;
  pthread_cond_broadcast(&coder->ready_coders_counter_m_c->cond);
  pthread_mutex_unlock(&coder->ready_coders_counter_m_c->mutex);

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == STARTING)
    pthread_cond_wait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return true;
}

bool coders_creator(t_representer *representer) 
{
  int i = 0;
  
  while (i < representer->config.number_of_coders) {
    if (pthread_create(&representer->coders[i]->thread, NULL,
                       routine_all_the_coders, representer->coders[i]) != 0) {
      // TODO: stop the running codders
      return false;
    }
    i++;
  }
  return true;
}

void coders_joiner(t_representer *representer) {
  int i;
  i = 0;
  while (i < representer->config.number_of_coders) {
    pthread_join(representer->coders[i]->thread, NULL);
    i++;
  }
}
