/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/01 06:02:45 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"
#include <sys/time.h>

bool wait_for_coders_to_start(t_representer *representer);
static void allow_coders_to_start(t_representer *representer);

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
  allow_coders_to_start(representer);

  while (1) {
    if (is_represontation_done(representer) ||
        are_one_of_coders_burnout(representer))
      break;
    else {
      usleep(200);
    }
  }
  exit_representation(representer);
  return NULL;
}

bool is_represontation_done(t_representer *representer) {
  bool is_done;
  is_done = false;
  pthread_mutex_lock(&representer->finished_coders_mutex);
  if (representer->finshed_coders == representer->config.number_of_coders)
    is_done = true;
  pthread_mutex_unlock(&representer->finished_coders_mutex);
  return is_done;
}

void exit_representation(t_representer *representer) {
  int i;
  i = 0;

  pthread_mutex_lock(&representer->is_burnout_mutex);
  representer->is_burnout = true;
  pthread_mutex_unlock(&representer->is_burnout_mutex);

  while (i < representer->config.number_of_coders) {
    pthread_mutex_lock(&representer->coders[i]->mutex_cond.mutex);
    representer->coders[i]->coder_state = EXIT;
    pthread_cond_broadcast(&representer->coders[i]->mutex_cond.cond);
    pthread_mutex_unlock(&representer->coders[i]->mutex_cond.mutex);
    i++;
  }
}

bool wait_for_coders_to_start(t_representer *representer) {
  pthread_mutex_lock(&representer->ready_coders_counter_m_c.mutex);
  while (representer->ready_coders_counter !=
         representer->config.number_of_coders) {
    pthread_cond_wait(&representer->ready_coders_counter_m_c.cond,
                      &representer->ready_coders_counter_m_c.mutex);
  }
  pthread_mutex_unlock(&representer->ready_coders_counter_m_c.mutex);
  return true;
}

static void allow_coders_to_start(t_representer *representer) {
  int i;
  t_coder **coders;

  i = 0;
  coders = representer->coders;
  gettimeofday(&representer->begining_time, NULL);
  while (i < representer->config.number_of_coders) {
    pthread_mutex_lock(&coders[i]->mutex_cond.mutex);
    gettimeofday(&coders[i]->last_compile, NULL);
    coders[i]->coder_state = WAIT;

    pthread_cond_broadcast(&coders[i]->mutex_cond.cond);
    pthread_mutex_unlock(&coders[i]->mutex_cond.mutex);
    i++;
  }
  representer->coders_are_ready = true;
}
