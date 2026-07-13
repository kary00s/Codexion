

#include "../codexion.h"

static long get_the_hotest_dongle(t_dongle *left_dongle, t_dongle *right_dongle);
static bool is_the_dongle_cold(t_dongle *dongle, unsigned long time_cooldown);




static bool is_the_dongle_cold(t_dongle *dongle, unsigned long time_cooldown) 
{
  unsigned long right_now;
  unsigned long time_get_cold;

  right_now = get_time_ms();
  time_get_cold = timeval_to_ms(dongle->last_reste) + time_cooldown;

  if (right_now > time_get_cold)
    return true;
  return false;
}

bool is_dongle_ready(t_dongle *dongle, unsigned long time_to_cooldown) {
  bool its_ready;
  its_ready = false;

  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  if (dongle->is_available)
    its_ready = true;
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
  return its_ready;
}

bool check_dongles_coldness(t_coder *coder) 
{
  long last_rest_for_hotest_dongle;
  if (!is_the_dongle_cold(coder->left_dongle,coder->config->dongle_cooldown) ||
      !is_the_dongle_cold(coder->right_dongle, coder->config->dongle_cooldown)) 
  {
    last_rest_for_hotest_dongle = get_the_hotest_dongle(coder->left_dongle, coder->right_dongle);
    return wait_dongles_to_cold(coder, last_rest_for_hotest_dongle);
  }
  return is_representation_works_well(coder->is_burnout_mutex,
                                      coder->is_burnout);
}

static long get_the_hotest_dongle(t_dongle *left_dongle, t_dongle *right_dongle)
{
  long time_spent_left;
  long time_spent_right;

  pthread_mutex_lock(&left_dongle->dongle_m_c.mutex);
  time_spent_left = timeval_to_ms(left_dongle->last_reste);
  pthread_mutex_unlock(&left_dongle->dongle_m_c.mutex);

  pthread_mutex_lock(&right_dongle->dongle_m_c.mutex);
  time_spent_right = timeval_to_ms(right_dongle->last_reste);
  pthread_mutex_unlock(&right_dongle->dongle_m_c.mutex);

  if (time_spent_left > time_spent_right)
    return time_spent_left;
  return time_spent_right;
}
