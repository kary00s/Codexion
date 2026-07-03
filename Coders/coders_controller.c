#include "../codexion.h"

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
