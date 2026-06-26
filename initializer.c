/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:52:28 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/15 21:05:22 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool initializer_queue(t_representer *representer);

bool init_representer_mutexs_conds(t_representer *representer) {
  if (pthread_mutex_init(&representer->print_mutex, NULL) != 0)
    return false;
  if (!init_mutex_cond(&representer->ready_coders_counter_m_c)) {
    pthread_mutex_destroy(&representer->print_mutex);
    return false;
  }
  // if (pthread_mutex_init(&representer->queue->mutex_queue, NULL) != 0) {
  //   pthread_mutex_destroy(&representer->print_mutex);
  //   destroy_mutex_cond(&representer->ready_coders_counter_m_c);
  //   return false;
  // }
  return true;
}

bool initialize_representer_struct(t_representer *representer, int ac,
                                   char **av) {
  if (!init_representer_mutexs_conds(representer))
    return false;
  representer->config = parser(ac, av);
  representer->coders_counter = 0;
  representer->coders_are_ready = false;
  representer->is_burnouted = false;
  representer->ready_coders_counter = 0;
  // if (!initializer_queue(representer)) {
  //   // TODO: clean the mutexes and conds
  //   return false;
  // }
  if (!init_dongles(representer)) {
    // TODO: destroy the mutexes and conds
    // TODO: destroy the mutexes and cond for queue and free memory
    return false;
  }
  if (!init_coders(representer)) {
    // TODO: free dongles and destroy the mutex in representer and queue
    return false;
  }
  return true;
}

static bool initializer_queue(t_representer *representer) {
  t_queue *queue;

  queue = malloc(sizeof(t_queue));
  if (queue == NULL)
    return false;
  queue->capacity = representer->config.number_of_coders;
  queue->coders = representer->coders;
  representer->queue = queue;
  queue->size = 0;
  return true;
}
