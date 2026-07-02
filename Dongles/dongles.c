
#include "../codexion.h"

static t_dongle **dongles_allocater(t_representer *representer) {
  t_dongle **dongles_list;
  int i;

  dongles_list = (t_dongle **)malloc(sizeof(t_dongle *) *
                                     representer->config.number_of_coders);
  if (!dongles_list)
    return (NULL);
  i = 0;
  while (i < representer->config.number_of_coders) {
    dongles_list[i] = (t_dongle *)malloc(sizeof(t_dongle));
    if (!dongles_list[i]) {
      while (i >= 0)
        free(dongles_list[i--]);
      free(dongles_list);
      return NULL;
    }
    i++;
  }
  return (dongles_list);
}

void dongles_destroyer(t_dongle **dongles, int counter)
{
  int i;
  i = 0;
  while (i < counter) {
    pthread_mutex_destroy(&dongles[i]->dongle_m_c.mutex);
    pthread_cond_destroy(&dongles[i]->dongle_m_c.cond);
    i++;
  }
}

static t_dongle **initialize_dongles_struct(t_dongle **dongles, int counter) {
  int i;

  i = 0;
  while (i < counter) {
    dongles[i]->is_available = true;
    dongles[i]->dongle_id = i;
    dongles[i]->last_reste = 0;
    dongles[i]->is_available = true;
    if (pthread_mutex_init(&dongles[i]->dongle_m_c.mutex, NULL)) {
      dongles_destroyer(dongles, i);
      printf("dongle error \n");
      return (NULL);
    }
    if (pthread_cond_init(&dongles[i]->dongle_m_c.cond, NULL)) {
      dongles_destroyer(dongles, i);
      printf("dongle error \n");
      return (NULL);
    }
    i++;
  }
  return (dongles);
}
bool is_the_dongle_cold(t_dongle *dongle, long time_cooldown)
{
  long right_now;
  long time_usage ;
  
  right_now = get_time_ms();
  time_usage = right_now - dongle->last_reste;
  if ((time_usage) > (time_cooldown))
      return true;
  return false;
}

bool is_dongle_ready(t_dongle *dongle, long time_to_cooldown) 
{
  bool its_ready;
  its_ready = false;

  
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  if ( (dongle->is_available)
  && (is_the_dongle_cold(dongle, time_to_cooldown)))
    its_ready = true;
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
  return its_ready;
}


bool are_dongles_available(t_coder *coder)
{
  
  if  (    (is_dongle_ready(coder->left_dongle, coder->config->dongle_cooldown))
  && (is_dongle_ready(coder->right_dongle, coder->config->dongle_cooldown))
)
{
  make_dongles_unavailable(coder->left_dongle);
  make_dongles_unavailable(coder->right_dongle);
  return true;
}
  return false;
}

void make_dongles_unavailable(t_dongle *dongle)
{
  pthread_mutex_lock(&dongle->dongle_m_c.mutex);
  dongle->is_cold = false; 
  dongle->is_available = false;                   
  pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
  
}
  // TODO: coder->right_dongle->last_reste = get_time_ms() + ;
bool init_dongles(t_representer *representer)
{
  representer->dongles = dongles_allocater(representer);
  if (!representer->dongles)
    return false;

  representer->dongles = initialize_dongles_struct(
      representer->dongles, representer->config.number_of_coders);
  if (!representer->dongles) {
    // TODO: free dongles
    return false;
  }
  return true;
}
