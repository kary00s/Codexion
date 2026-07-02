
#include "../codexion.h"

void *controller_home(void *args);
bool controller_creator(t_representer *representer) 
{
  if (pthread_create(&representer->controller, NULL, &controller_home, representer))
    return false;
  return true;
}

void controller_joiner(pthread_t *controller)
{
  pthread_join(*controller, NULL);
  return;
}

void *controller_home(void *args) 
{
  t_representer *representer;
  representer = (t_representer *)args;
  t_coder *coder;

  while (1) 
  {
    coder = catch_coder(representer);
    if (coder != NULL && coder->coder_state == WAITING) { 
      printf("coder picked \n");
      pthread_mutex_lock(&coder->mutex_cond.mutex);
      coder->coder_state = COMPILING;
      pthread_cond_broadcast(&coder->mutex_cond.cond);
      pthread_mutex_unlock(&coder->mutex_cond.mutex);
    }
    else
      usleep(100);
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
    if (are_dongles_available(representer->coders[i])) 
    {
      coder = representer->coders[i];
      pop_coder_from_queue(representer, i);
      pthread_mutex_unlock(&representer->queue->mutex_queue);
      return coder;
    }
    i++;
  }
  pthread_mutex_unlock(&representer->queue->mutex_queue);
  return coder;
}
