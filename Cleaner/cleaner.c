#include "../codexion.h"

void free_dongles(t_representer *representer);
void clean_initialize_representer_struct(t_representer *representer);
void free_previous_coders(t_coder **coders, int n) ;


void free_dongles(t_representer *representer) 
{
  int i;

  i = 0;
  while ((representer->dongles[i] != NULL) &&
         (i <= representer->config.number_of_coders))
    free(representer->dongles[i++]);
  free(representer->dongles);
}

void free_coders(t_coder **coders) 
{
  int i;

  i = 0;
  while ((coders[i] != NULL) &&
         (i <= coders[i]->config->number_of_coders))
    free(coders[i++]);
  free(coders);
}


void clean_initialize_representer_struct(t_representer *representer)
{
  // TODO: stop the running coders and free the resources
  dongles_mutexes_destroyer(re);
  destroy_mutex_coders(&(*representer->coders), representer->config.number_of_coders);
  free_coders(representer->coders);
  free_dongles(representer);
}

void free_previous_coders(t_coder **coders, int n) 
{
  int i;

  i = 0;
  while (i < n) 
  {
    free(coders[i]);
    i++;
  }
  free(coders);
}