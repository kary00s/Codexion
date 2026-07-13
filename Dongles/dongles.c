
#include "../codexion.h"
static t_dongle **initialize_dongles_struct(t_dongle **dongles, int counter);
static t_dongle **dongles_allocater(t_representer *representer);

static t_dongle **dongles_allocater(t_representer *representer)
{
  t_dongle **dongles_list;
  int i;

  dongles_list = (t_dongle **)malloc(sizeof(t_dongle *) *
                                     representer->config.number_of_coders);
  if (!dongles_list)
    return (NULL);
  i = 0;
  while (i < representer->config.number_of_coders) {
    dongles_list[i] = (t_dongle *)malloc(sizeof(t_dongle));
    if (!dongles_list[i])
    {
      while (i >= 0)
        free(dongles_list[i--]);
      free(dongles_list);
      return NULL;
    }
    i++;
  }
  return (dongles_list);
}

bool init_dongles(t_representer *representer) 
{
  representer->dongles = dongles_allocater(representer);
  if (!representer->dongles)
    return false;
  
  representer->dongles = initialize_dongles_struct(representer->dongles, representer->config.number_of_coders);
  if (!representer->dongles) 
  {
    free_dongles(representer);
    return false;
  }
  return true;
}
  
static t_dongle **initialize_dongles_struct(t_dongle **dongles, int counter) {
  int i;

  i = 0;
  while (i < counter) {
    dongles[i]->is_available = true;
    dongles[i]->dongle_id = i;
    if (!init_mutex_cond(&dongles[i]->dongle_m_c)) 
    {
      dongles_mutexes_destroyer(dongles, i);
      return (NULL);
    }
    i++;
  }
  return (dongles);
}

