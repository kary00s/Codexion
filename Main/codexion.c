#include "../codexion.h"

int main(int ac, char *av[]) {
  t_representer representer;
  if (!initialize_representer_struct(&representer, ac, av))
    return 0;

  if (!monitor_creator(&representer))
    return 0;
  
  if (!coders_creator(&representer)) 
    return 0;
    
  if (!controller_creator(&representer)) 
    return 0;
    
  monitor_joiner(&representer.monitor);
  coders_joiner(&representer);
  controller_joiner(&representer.controller);
  
  clean_initialize_representer_struct(&representer);
}
