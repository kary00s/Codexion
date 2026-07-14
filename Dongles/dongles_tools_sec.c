#include"../codexion.h"



bool are_dongles_available(t_coder *coder) 
{
  if ((is_dongle_ready(coder->left_dongle)) &&
      (is_dongle_ready(coder->right_dongle))) 
  {
    make_dongles_unavailable(coder->right_dongle);
    make_dongles_unavailable(coder->left_dongle);
    return true;
  }
  return false;
}

void make_dongles_unavailable(t_dongle *dongle) {
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  dongle->is_available = false;
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

void drop_both_dongles(t_coder *coder)
{
  pthread_mutex_lock(&coder->left_dongle->dongle_m_c.mutex);
  coder->left_dongle->is_available = true;
  gettimeofday(&coder->left_dongle->last_reste, NULL);
  pthread_mutex_unlock(&coder->left_dongle->dongle_m_c.mutex);

  pthread_mutex_lock(&coder->right_dongle->dongle_m_c.mutex);
  coder->right_dongle->is_available = true;
  gettimeofday(&coder->right_dongle->last_reste, NULL);
  pthread_mutex_unlock(&coder->right_dongle->dongle_m_c.mutex);
}


bool wait_dongles_to_cold(t_coder *coder, long cooldown_time)
{
  unsigned long time_to_get_cold;
  time_to_get_cold = cooldown_time + coder->config->dongle_cooldown;
  return wait(&coder->mutex_cond.mutex, &coder->mutex_cond.cond,
              time_to_get_cold);
}
