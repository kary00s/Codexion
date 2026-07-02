#include "../codexion.h"

void free_dongles(t_representer *representer) 
{
  int i;

  i = 0;
  while ((representer->dongles[i] != NULL) &&
         (i <= representer->config.number_of_coders))
    free(representer->dongles[i++]);
  free(representer->dongles);
}

void free_coders(t_representer *representer) 
{
  int i;

  i = 0;
  while ((representer->coders[i] != NULL) &&
         (i <= representer->config.number_of_coders))
    free(representer->coders[i++]);
  free(representer->coders);
}


void clean_initialize_representer_struct(t_representer *representer)
{ 
  // TODO: stop the running coders and free the resources
  destroy_mutex_coders(&(*representer->coders), representer->config.number_of_coders);
  free_coders(representer);
  free_dongles(representer);
}