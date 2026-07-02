
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
