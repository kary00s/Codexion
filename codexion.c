#include "codexion.h"
#include "codexion.h"

int main(int ac, char *av[])
{
  t_representer representer;
  
  if (!initialize_representer_struct(&representer, ac, av))
  {
    return 0;  
  }
  
  if (!manager_creator(&representer)) {
    // TODO: stop the running coders and free the resources
    
    destroy_mutex_coders(&(*representer.coders), representer.config.number_of_coders);
    free_coders(&representer);
    free_dongles(&representer);
    return 0;
  }
  if (!monitor_creator(&representer)) {
    // TODO: stop the running coders and free the resources
    
    destroy_mutex_coders(&(*representer.coders), representer.config.number_of_coders);
    free_coders(&representer);
    free_dongles(&representer);
    return 0;
  }
  
  if (!coders_creator(&representer)) {
    return 0;
  }
  printf("=== all good untill here ===\n");
  
  coders_joiner(&representer);
  monitor_joiner(&representer.monitor);
  manager_joiner(&representer.manager);
  
}