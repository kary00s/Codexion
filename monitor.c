/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/16 09:40:27 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool wait_for_coders_to_start(t_representer *representer);

bool monitor_creator(t_representer *representer) {
  if (pthread_create(&representer->monitor, NULL, &monitor_home, representer))
    return false;
  return true;
}

void monitor_joiner(pthread_t *monitor) {
  pthread_join(*monitor, NULL);
  return;
}

void *monitor_home(void *args) {
  t_representer *representer;
  representer = (t_representer *)args;
  wait_for_coders_to_start(representer);

  return NULL;
}

static bool wait_for_coders_to_start(t_representer *representer) {
  pthread_mutex_lock(&representer->ready_coders_counter_m_c.mutex);
  while (representer->ready_coders_counter !=
         representer->config.number_of_coders) {
    pthread_cond_wait(&representer->ready_coders_counter_m_c.cond,
                      &representer->ready_coders_counter_m_c.mutex);
  }
  pthread_mutex_unlock(&representer->ready_coders_counter_m_c.mutex);
  printf("all cooders are ready %d\n", representer->ready_coders_counter);
  return true;
}
