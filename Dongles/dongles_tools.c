

#include "../codexion.h"
static long get_the_hotest_dongle(t_dongle *left_dongle, t_dongle *right_dongle);
static bool wait_dongles_to_cold(t_coder *coder, long cooldown_time);
static void drop_dongle(t_dongle *dongle);
static bool is_dongle_ready(t_dongle *dongle, unsigned long time_to_cooldown);
static bool is_the_dongle_cold(t_dongle *dongle, unsigned long time_cooldown);

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

bool are_dongles_available(t_coder *coder) 
{
  if ((is_dongle_ready(coder->left_dongle, coder->config->dongle_cooldown)) &&
      (is_dongle_ready(coder->right_dongle, coder->config->dongle_cooldown))) {
    make_dongles_unavailable(coder->right_dongle);
    make_dongles_unavailable(coder->left_dongle);
    return true;
  }
  return false;
}


bool wait_dongles_to_cold(t_coder *coder, long cooldown_time) {
  unsigned long time_to_get_cold;
  time_to_get_cold = cooldown_time + coder->config->dongle_cooldown;
  return wait(&coder->mutex_cond.mutex, &coder->mutex_cond.cond,
              time_to_get_cold);
}

void make_dongles_unavailable(t_dongle *dongle) {
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  dongle->is_available = false;
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

static void drop_dongle(t_dongle *dongle) {
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  dongle->is_available = true;
  dongle->is_cold = false;
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

static bool is_the_dongle_cold(t_dongle *dongle, unsigned long time_cooldown) {
  unsigned long right_now;
  unsigned long time_get_cold;

  right_now = get_time_ms();
  time_get_cold = timeval_to_ms(dongle->last_reste) + time_cooldown;

  if (right_now > time_get_cold)
    return true;
  return false;
}

static bool is_dongle_ready(t_dongle *dongle, unsigned long time_to_cooldown) {
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
