#include "../codexion.h"

void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles, int number_of_coders)
{
  int i;

  i = 0;
  while (i < number_of_coders) {
    coders[i]->left_dongle = dongles[i];
    coders[i]->right_dongle = dongles[(i + 1) % number_of_coders];
    i++;
  }
}

void coders_joiner(t_representer *representer)
{
  int i;
  i = 0;
  while (i < representer->config.number_of_coders) {
    pthread_join(representer->coders[i]->thread, NULL);
    i++;
  }
}

bool coder_waiting_dongles(t_coder *coder) {

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == WAIT)
    pthread_cond_wait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return is_representation_works_well(coder->is_burnout_mutex,
                                      coder->is_burnout);
}

bool are_one_of_coders_burnout(t_representer *representer) {
  int i;

  i = 0;
  while (i < representer->config.number_of_coders) {
    if (is_coder_burnouted(representer->coders[i]))
      return true;
    i++;
  }
  return false;
}

bool is_coder_burnouted(t_coder *coder) {
  bool is_burnouted;
  is_burnouted = false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);

  long time_esplited = time_elapsed_until_now(coder->last_compile);
  if (time_esplited >= coder->config->time_to_burnout) {
    is_burnouted = true;
    coder->coder_state = EXIT;
    print_action(coder);
  }
  pthread_mutex_unlock(&coder->mutex_cond.mutex);

  return is_burnouted;
}

t_coder *catch_coder(t_representer *representer) 
{
  t_coder *coder;
  int i = 0;
  coder = NULL;

  pthread_mutex_lock(&representer->queue->mutex_queue);
  while (i < representer->queue->size) {
    if (are_dongles_available(representer->queue->coders[i])) {
      coder = representer->queue->coders[i];
      if (!is_representation_works_well(&representer->is_burnout_mutex,
                                        &representer->is_burnout))
      {
        pthread_mutex_unlock(&representer->queue->mutex_queue);
        return NULL;
      }
      pop_coder_from_queue(representer, i);
      break;
    }
    i++;
  }
  pthread_mutex_unlock(&representer->queue->mutex_queue);
  return coder;
}
