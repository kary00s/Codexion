/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/13 04:52:50 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"
#include <sys/time.h>

bool monitor_creator(t_representer *representer) 
{
  if (pthread_create(&representer->monitor, NULL, &monitor_home, representer))
  {
    exit_representation(representer);
    broadcast_coders_to_exit(representer, representer->config.number_of_coders);
    representer_mutexes_destroyer(representer);
    clean_initialize_representer_struct(representer);
    return false;
  }
  return true;
}

void monitor_joiner(pthread_t *monitor) 
{
  pthread_join(*monitor, NULL);
  return;
}

void *monitor_home(void *args) 
{
  t_representer *representer;
  representer = (t_representer *)args;

  wait_for_coders_to_start(representer);
  allow_coders_to_start(representer);

  while (1) {
    if (is_represontation_done(representer) ||
        are_one_of_coders_burnout(representer))
      break;
    else {
      usleep(500);
    }
  }
  exit_representation(representer);
  broadcast_coders_to_exit(representer, representer->config.number_of_coders);
  return NULL;
}




