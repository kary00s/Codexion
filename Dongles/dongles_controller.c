

#include"../codexion.h"


bool hold_both_dongles(t_coder *coder);
bool drop_both_dongles(t_coder *coder);
bool are_dongles_available(t_coder *coder);
bool wait_dongles_to_cold(t_dongle *dongle, long time_to_get_cold);

static void drop_dongle(t_dongle *dongle);
static void hold_dongle(t_dongle *dongle);
static bool is_the_dongle_cold(t_dongle *dongle, long time_cooldown);
static bool is_dongle_ready(t_dongle *dongle, long time_to_cooldown) ;

bool hold_both_dongles(t_coder *coder)
{  

  if (are_dongles_available(coder))
  {
    printf("========= dngles are avail ============\n");

      hold_dongle(coder->left_dongle);
    	hold_dongle(coder->right_dongle);

      pthread_mutex_lock(coder->print_mutex);
      printf("%d has taken dongles\n", coder->coder_id );
      pthread_mutex_unlock(coder->print_mutex);

      return true;
  }
  return false;
}

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
  {
    make_dongles_unavailable(coder->left_dongle);
    make_dongles_unavailable(coder->right_dongle);
    return true;
  printf("========= dongle holded======\n");
  
  }
  return false;
}

bool wait_dongles_to_cold(t_dongle *dongle, long time_to_get_cold)
{
  long right_now;
  right_now = get_time_ms();
  time_to_get_cold = dongle->last_reste + time_to_get_cold;
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  while (1)
  {
    right_now = get_time_ms();
    if (right_now < time_to_get_cold)
      break;
    else
      usleep(50);
  }
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
  printf("========= dongle holded======\n");
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = false;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}

static void drop_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = true;
    dongle->last_reste =  get_time_ms();
    dongle->is_cold = false;
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
    printf("==================\n");
    if (is_the_dongle_cold(dongle, time_to_cooldown))
      its_ready = true;
    else
      wait_dongles_to_cold(dongle ,time_to_cooldown);
  }
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
  return its_ready;
}
