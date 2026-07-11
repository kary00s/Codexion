#include "../codexion.h"

static long ft_atol(char *str) {
  int i;
  i = 0;
  unsigned long res;
  res = 0;
  if (!str)
    return 42;

  while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
    i++;
  while (str[i]) {
    if (((str[i] >= '0') && (str[i] <= '9')) && (res < LONG_MAX))
      res = 10 * res + str[i] - 48;

    else
      return LONG_MAX;
    i++;
  }
  return res;
}


int ft_strcmp(char *s1, char *s2)
{
  int i;
  i = 0;

  while (s1[i] && s2[i] && (s1[i] - s2[i] == 0))
    i++;
  return (s1[i] - s2[i]);
}

bool parser(int ac, char **args, t_representer *representer)
{
  t_config config;
  if (ac == 9) {
    config.number_of_coders = ft_atol(args[1]);
    if(config.number_of_coders == LONG_MAX)
      return false;
    config.time_to_burnout = ft_atol(args[2]);
    if(config.time_to_burnout == LONG_MAX)
      return false;
    config.time_to_compile = ft_atol(args[3]);
    if(config.time_to_compile == LONG_MAX)
      return false;
    config.time_to_debug = ft_atol(args[4]);
    if(config.time_to_debug == LONG_MAX)
      return false;
    config.time_to_refactor = ft_atol(args[5]);
    if(config.time_to_refactor == LONG_MAX)
      return false;
    config.number_of_compiles_required = ft_atol(args[6]);
    if(config.number_of_compiles_required == LONG_MAX)
      return false;
    config.dongle_cooldown = ft_atol(args[7]);
    if(config.dongle_cooldown == LONG_MAX)
      return false;
    if (ft_strcmp(args[8], "edf") == 0)
      config.scheduler = EDF;
    else if (ft_strcmp(args[8], "fifo") == 0)
      config.scheduler = FIFO;
    else
    {
      printf("the schudeler must be 'edf' of 'fifo' \n");
      return false;
    }
  }
  else 
    return false;
  representer->config = config;
  return true;
}
