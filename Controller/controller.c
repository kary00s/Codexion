
#include "../codexion.h"

void *controller_home(void *args);
bool controller_creator(t_representer *representer) {
  if (pthread_create(&representer->controller, NULL, &controller_home,
                     representer))
    return false;
  return true;
}

bool is_representation_works_well(t_representer *representer)
{
  bool works_well;
  works_well = true;
  pthread_mutex_lock(&representer->m_c.mutex);
  if (representer->is_burnout == true)
     works_well = false;  
  pthread_mutex_unlock(&representer->m_c.mutex);
  return works_well;
}

void controller_joiner(pthread_t *controller) {
  pthread_join(*controller, NULL);
  return;
}

void *controller_home(void *args) {
  t_representer *representer;
  t_coder *coder;
  int i = 0;

  representer = (t_representer *)args;
  while (!is_represontation_done(representer))
  {
    coder = catch_coder(representer);
    if (coder) {
      pthread_mutex_lock(&coder->mutex_cond.mutex);
      coder->coder_state = COMPILING;
      pthread_cond_broadcast(&coder->mutex_cond.cond);
      pthread_mutex_unlock(&coder->mutex_cond.mutex);
    }
    else
      usleep(200);
  }
  return NULL;
}

t_coder *catch_coder(t_representer *representer)
{
  t_coder *coder;
  int i = 0;
  coder = NULL;

  pthread_mutex_lock(&representer->queue->mutex_queue);
  while (i < representer->queue->size) 
  {
    if (are_dongles_available(representer->queue->coders[i])) 
    {
      coder = representer->queue->coders[i];
      if (!is_representation_works_well(representer))
        return NULL;
      pop_coder_from_queue(representer, i);
      break;
    }
    i++;
  }
  pthread_mutex_unlock(&representer->queue->mutex_queue);
  return coder;
}

bool is_coder_in_exit_state(t_coder *coder)
{
  bool is_it;
  is_it = false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  if(coder->coder_state == EXIT)
    is_it = true;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);

  return is_it;
}
