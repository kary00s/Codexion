#include "../codexion.h"

bool controller_creator(t_representer *representer) 
{
  if (pthread_create(&representer->controller, NULL, &controller_home,
                     representer))
    return false;
  return true;
}

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

void controller_joiner(pthread_t *controller) {
  pthread_join(*controller, NULL);
  return;
}

void *controller_home(void *args) {
  t_representer *representer;
  t_coder *coder;
  int i = 0;

  representer = (t_representer *)args;
  while (is_representation_works_well(&representer->is_burnout_mutex,
                                      &representer->is_burnout)) {
    coder = catch_coder(representer);
    if (coder)
    {
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

bool wait(pthread_mutex_t *mutex, pthread_cond_t *cond, unsigned long time) {
  t_timespec time_spec;
  bool is_ok;

  is_ok = true;
  pthread_mutex_lock(mutex);
  ms_to_timespec(&time_spec, time);
  if (pthread_cond_timedwait(cond, mutex, &time_spec) != ETIMEDOUT)
    is_ok = false;
  pthread_mutex_unlock(mutex);
  return is_ok;
}
