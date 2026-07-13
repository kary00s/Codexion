#include"../codexion.h"

bool wait_for_coders_to_start(t_representer *representer) 
{
  pthread_mutex_lock(&representer->ready_coders_counter_m_c.mutex);
  while (representer->ready_coders_counter !=
         representer->config.number_of_coders) {
    pthread_cond_wait(&representer->ready_coders_counter_m_c.cond,
                      &representer->ready_coders_counter_m_c.mutex);
  }
  pthread_mutex_unlock(&representer->ready_coders_counter_m_c.mutex);
  return true;
}

void allow_coders_to_start(t_representer *representer)
{
  int i;
  t_coder **coders;

  i = 0;
  coders = representer->coders;
  gettimeofday(&representer->begining_time, NULL);
  while (i < representer->config.number_of_coders)
  {
    pthread_mutex_lock(&coders[i]->mutex_cond.mutex);
    gettimeofday(&coders[i]->last_compile, NULL);
    coders[i]->coder_state = WAIT;

    pthread_cond_broadcast(&coders[i]->mutex_cond.cond);
    pthread_mutex_unlock(&coders[i]->mutex_cond.mutex);
    i++;
  }
}

void broadcast_coders_to_exit(t_representer *representer, int counter)
{
  int i = 0;
  while (i < counter) 
  {
    pthread_mutex_lock(&representer->coders[i]->mutex_cond.mutex);
    representer->coders[i]->coder_state = EXIT;
    pthread_cond_broadcast(&representer->coders[i]->mutex_cond.cond);
    pthread_mutex_unlock(&representer->coders[i]->mutex_cond.mutex);
    i++;
  }
}

void swap_coders(t_coder **parent_coder, t_coder **child_coder) 
{
  t_coder *swp;
  swp = *parent_coder;
  *parent_coder = *child_coder;
  *child_coder = swp;
}