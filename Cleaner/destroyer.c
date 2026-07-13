#include"../codexion.h"

void dongles_mutexes_destroyer(t_dongle **dongles, int counter)
{
  int i;
  i = 0;
  while (i < counter)
  {
    destroy_mutex_cond(&dongles[i]->dongle_m_c);
    i++;
  }
}

void destroy_mutex_cond(t_mutex_cond *mutex_cond)
{
  pthread_mutex_destroy(&mutex_cond->mutex);
  pthread_cond_destroy(&mutex_cond->cond);
}

void destroy_mutex_coders(t_coder **coders, int n)
{
  int i;

  i = 0;
  while (i < n) {
    destroy_mutex_cond(&coders[i]->mutex_cond);
    i++;  
  }
}

void representer_mutexes_destroyer(t_representer *representer)
{
  pthread_mutex_destroy(&representer->is_burnout_mutex);
  pthread_mutex_destroy(&representer->finished_coders_mutex);
  pthread_mutex_destroy(&representer->print_mutex);
  destroy_mutex_cond(&representer->ready_coders_counter_m_c);
}

