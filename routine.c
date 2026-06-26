#include "codexion.h"
#include <pthread.h>

static bool wait_for_simulation_to_start(t_coder *coder);

void *routine_all_the_coders(void *arg) {
  t_coder *coder;
  coder = (t_coder *)arg;
  wait_for_simulation_to_start(coder);
  pthread_mutex_lock(coder->print_mutex);
  printf("the coder is starting now id: %d\n", coder->coder_id);
  pthread_mutex_unlock(coder->print_mutex);
  return NULL;
}

static bool wait_for_simulation_to_start(t_coder *coder) {
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

bool coders_creator(t_representer *representer) {
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
