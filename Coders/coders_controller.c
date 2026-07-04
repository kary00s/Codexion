#include "../codexion.h"
static bool is_coder_burnouted(t_coder *coder);

void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
                                int number_of_coders);
bool coder_waiting_dongles(t_coder *coder);

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

bool coder_waiting_dongles(t_coder *coder) {
  bool done;

  done = true;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == WAITING)
    pthread_cond_wait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);

  return (done);
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
      pthread_mutex_lock(&representer->coders[i]->burnout_mutex);
      burnouted = true;
      representer->coders[i]->is_burnouted = true;
      pthread_mutex_unlock(&representer->coders[i]->burnout_mutex);
      return burnouted;
    }
    i++;
  }

  return burnouted;
}

static bool is_coder_burnouted(t_coder *coder)
{
  bool is_burnouted;
   is_burnouted = false; 
  unsigned long time_spent;
  pthread_mutex_lock(&coder->burnout_mutex);
  
  time_spent = get_time_ms();
  time_spent -= timeval_to_ms(coder->last_compile); 
  
  if (time_spent > coder->config->time_to_burnout)
  {
    coder->is_burnouted = true;
    is_burnouted = true;
  }
  pthread_mutex_unlock(&coder->burnout_mutex);
  return is_burnouted;
}
