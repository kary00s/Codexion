#include "codexion.h"
#include <pthread.h>

bool init_mutex_cond(t_mutex_cond *mutex_cond) {
  if (pthread_mutex_init(&mutex_cond->mutex, NULL) != 0)
    return false;
  if (pthread_cond_init(&mutex_cond->cond, NULL) != 0) {
    pthread_mutex_destroy(&mutex_cond->mutex);
    return false;
  }
  return true;
}

void destroy_mutex_cond(t_mutex_cond *mutex_cond) {
  printf("hello karim\n");
  pthread_mutex_destroy(&mutex_cond->mutex);
  pthread_cond_destroy(&mutex_cond->cond);
}
