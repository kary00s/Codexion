/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:52:28 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/03 09:44:27 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"


bool initialize_representer_struct(t_representer *representer, int ac, char **av) 
{
  
  representer->is_burnout = false;
  representer->finshed_coders = 0;
  representer->ready_coders_counter = 0;

  if (!parser(ac, av, representer))
    return false;
  if (!init_representer_mutexs_conds(representer))
    return false;
  if (!init_queue(representer))
  {
    pthread_mutex_destroy(&representer->print_mutex);
    destroy_mutex_cond(&representer->ready_coders_counter_m_c.mutex,
                       &representer->ready_coders_counter_m_c.cond);
    return false;
  }

  if (!init_dongles(representer)) {
    pthread_mutex_destroy(&representer->print_mutex);
    pthread_mutex_destroy(&representer->queue->mutex_queue);
    destroy_mutex_cond(&representer->ready_coders_counter_m_c.mutex,
                       &representer->ready_coders_counter_m_c.cond);
    free(representer->queue);
    return false;
  }

  if (!init_coders(representer)) {
    free_dongles(representer);
    pthread_mutex_destroy(&representer->print_mutex);
    pthread_mutex_destroy(&representer->queue->mutex_queue);
    destroy_mutex_cond(&representer->ready_coders_counter_m_c.mutex,
                       &representer->ready_coders_counter_m_c.cond);
    return false;
  }
  
  linker_coders_with_dongles(representer->coders, representer->dongles, representer->config.number_of_coders);
  return true;
}

bool init_representer_mutexs_conds(t_representer *representer) {
  if (pthread_mutex_init(&representer->print_mutex, NULL) != 0)
    return false;
  if (!init_mutex_cond(&representer->ready_coders_counter_m_c)) {
    pthread_mutex_destroy(&representer->print_mutex);
    return false;
  }
  if (pthread_mutex_init(&representer->finished_coders_mutex, NULL) != 0) {
    pthread_mutex_destroy(&representer->print_mutex);
    return false;
  }
  if (pthread_mutex_init(&representer->is_burnout_mutex, NULL) != 0) {
    // TODO: destroy predefined mutexes and conds
  }
  return true;
}
