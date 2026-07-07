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

static bool initializer_queue(t_representer *representer);
static bool init_queue_mutexs_conds(t_representer *representer);


bool init_representer_mutexs_conds(t_representer *representer)
{
  if (pthread_mutex_init(&representer->print_mutex, NULL) != 0)
    return false;
  if (!init_mutex_cond(&representer->ready_coders_counter_m_c))
  {
    pthread_mutex_destroy(&representer->print_mutex);
    return false;
  }
  if(!init_mutex_cond(&representer->required_numbers_compilation_is_completed_m_c))
  {
    pthread_mutex_destroy(&representer->ready_coders_counter_m_c.mutex);
    pthread_mutex_destroy(&representer->print_mutex);
  }
  return true;
}

bool initialize_representer_struct(t_representer *representer, int ac, char **av) 
{
  if(!parser(ac, av, representer))
    return false;
  
  if (!init_representer_mutexs_conds(representer))
    return false;
  representer->is_burnout = false;
  representer->coders_are_ready = false;
  representer->ready_coders_counter = 0;
  representer->required_numbers_compilation_is_completed = 0;
  

  if (!initializer_queue(representer) || !init_queue_mutexs_conds(representer)) 
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

static bool init_queue_mutexs_conds(t_representer *representer) 
{
  if (pthread_mutex_init(&representer->queue->mutex_queue, NULL) != 0) {
    pthread_mutex_destroy(&representer->print_mutex);
    destroy_mutex_cond(&representer->ready_coders_counter_m_c.mutex
                        , &representer->ready_coders_counter_m_c.cond);
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
  queue->size = 0;
  queue->coders = coders_allocater(queue->capacity);
  if (queue->coders == NULL) {
    free(queue);
    return false;
  }
  representer->queue = queue;
  return true;
}
