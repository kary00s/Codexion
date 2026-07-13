#include "../codexion.h"

void free_dongles(t_representer *representer) 
{
  int i;

  i = 0;
  while (i < representer->config.number_of_coders)
    free(representer->dongles[i++]);
  free(representer->dongles);
}

void free_coders(t_coder **coders, int  n) 
{
  int i;

  i = 0;
  while ((i < n))
  {
    free(coders[i]);
    i++;
  }
  free(coders);
}


void clean_initialize_representer_struct(t_representer *representer)
{
  clean_coders(representer);
  clean_queue(representer);
  clean_dongles(representer);
}



void clean_queue(t_representer *representer)
{
  pthread_mutex_destroy(&representer->queue->mutex_queue);
  free(representer->queue->coders);
  free(representer->queue);
}

void clean_coders(t_representer *representer)
{
  destroy_mutex_coders(representer->coders, representer->config.number_of_coders);  
  free_coders(representer->coders, representer->config.number_of_coders);
}

void clean_dongles(t_representer *representer)
{
  dongles_mutexes_destroyer(representer->dongles, representer->config.number_of_coders);
  free_dongles(representer);
}
