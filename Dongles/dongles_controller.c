

#include"../codexion.h"


bool drop_both_dongles(t_coder *coder);
bool are_dongles_available(t_coder *coder);
bool wait_dongles_to_cold(t_dongle *dongle, long time_to_get_cold);

static void drop_dongle(t_dongle *dongle);
static void hold_dongle(t_dongle *dongle);
static bool is_the_dongle_cold(t_dongle *dongle, long time_cooldown);
static bool is_dongle_ready(t_dongle *dongle, long time_to_cooldown) ;



bool drop_both_dongles(t_coder *coder)
{
	
  drop_dongle(coder->right_dongle);
	drop_dongle(coder->left_dongle);

  return true;
}


bool are_dongles_available(t_coder *coder)
{
  if ((is_dongle_ready(coder->left_dongle, coder->config->dongle_cooldown))
        && (is_dongle_ready(coder->right_dongle, coder->config->dongle_cooldown)))
  
    return true;
  return false;
}

bool wait_dongles_to_cold(t_dongle *dongle, long time_to_get_cold)
{
  t_timespec colding_time;
  time_to_get_cold = dongle->last_reste + time_to_get_cold;
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);  
  ms_to_timespec(&colding_time, time_to_get_cold);
  pthread_cond_timedwait(&dongle->dongle_m_c.cond, &dongle->dongle_m_c.mutex, &colding_time);
  dongle->is_cold = true;

  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  return true;
}

void make_dongles_unavailable(t_dongle *dongle)
{
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  dongle->is_cold = false; 
  dongle->is_available = false;                   
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

static void hold_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = false;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

static void drop_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = true;
    dongle->is_cold = true;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

static bool is_the_dongle_cold(t_dongle *dongle, long time_cooldown)
{
  long right_now;
  long time_usage ;
  long time_get_cold;
  right_now = get_time_ms();
  time_get_cold = dongle->last_reste + time_cooldown;
  if (right_now > time_get_cold)
      return true;
  return false;
  }

static bool is_dongle_ready(t_dongle *dongle, long time_to_cooldown) 
{
  bool its_ready;
  its_ready = false;
  
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  if (dongle->is_available)
  {
    if (is_the_dongle_cold(dongle, time_to_cooldown))
      its_ready = true;
    // else
    //   wait_dongles_to_cold(dongle ,time_to_cooldown);
  }
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
  return its_ready;
}
