#include "codexion.h"

int main(int ac, char *av[]) {
  t_representer representer;

  if (!initialize_representer_struct(&representer, ac, av))
    return 0;

  if (!monitor_creator(&representer)) {
    // TODO: stop the running codes and free the resources
    return 0;
  }

  if (!coders_creator(&representer)) {
    return 0;
  }

  coders_joiner(&representer);
  monitor_joiner(&representer.monitor);
}
