#include"../codexion.h"




bool is_representation_works_well(pthread_mutex_t *is_burnout_mutex, bool *is_burnout) 
{
  bool works_well;

  works_well = true;
  pthread_mutex_lock(is_burnout_mutex);
  if (*is_burnout == true)
    works_well = false;
  pthread_mutex_unlock(is_burnout_mutex);
  return works_well;
}

bool is_represontation_done(t_representer *representer) 
{
  bool is_done;
  is_done = false;
  pthread_mutex_lock(&representer->finished_coders_mutex);
  if (representer->finshed_coders == representer->config.number_of_coders)
    is_done = true;
  pthread_mutex_unlock(&representer->finished_coders_mutex);
  return is_done;
}

void exit_representation(t_representer *representer) {
  
  pthread_mutex_lock(&representer->is_burnout_mutex);
  representer->is_burnout = true;
  pthread_mutex_unlock(&representer->is_burnout_mutex);
}

bool wait_for_representation_to_start(t_coder *coder)
{
  pthread_mutex_lock(&coder->ready_coders_counter_m_c->mutex);
  (*coder->ready_coders_counter)++;
  pthread_cond_broadcast(&coder->ready_coders_counter_m_c->cond);
  pthread_mutex_unlock(&coder->ready_coders_counter_m_c->mutex);

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == START)
    pthread_cond_wait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return true;
}