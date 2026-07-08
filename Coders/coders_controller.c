#include "../codexion.h"
bool   is_coder_burnouted(t_coder *coder);

void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
                                int number_of_coders);
void coder_waiting_dongles(t_coder *coder);
void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
                                int number_of_coders) {
  int i;
  i = 0;
  while (i < number_of_coders) {
    coders[i]->left_dongle = dongles[i];
    coders[i]->right_dongle = dongles[(i + 1) % number_of_coders];
    i++;
  }
}

void coder_waiting_dongles(t_coder *coder) {

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == WAIT)
    pthread_cond_wait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
}


bool are_one_of_coders_burnout(t_representer *representer)
{
  int i ;
  i = 0;
  bool burnouted;
  burnouted = false;
  while (i < representer->config.number_of_coders)
  {
    if(is_coder_burnouted(representer->coders[i]))
    {
      pthread_mutex_lock(&representer->m_c.mutex);
      representer->is_burnout = false;
      burnouted = true;
      pthread_mutex_unlock(&representer->m_c.mutex);
      return burnouted;
    }
    i++;
  }

  return burnouted;
}

bool is_coder_burnouted(t_coder *coder)
{
  bool is_burnouted;
  is_burnouted = false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  
  long time_esplited = time_elapsed_until_now(coder->last_compile);
  if (time_esplited >= coder->config->time_to_burnout)
  {
    is_burnouted = true;
    printf("==========> %ld %d coder burnouted\n", time_esplited ,coder->coder_id);
    coder->coder_state = EXIT;
  }
  pthread_mutex_unlock(&coder->mutex_cond.mutex);

  return is_burnouted;
}