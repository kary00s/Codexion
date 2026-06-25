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
#include <pthread.h>


bool init_representer_mutexs_conds(t_representer *representer) {
  if (pthread_mutex_init(&representer->print_mutex, NULL) != 0)
    return false;
  if (init_mutex_cond(&representer->ready_coders_counter_m_c)) {
    pthread_mutex_destroy(&representer->print_mutex);
    return false;
  }
  if (pthread_mutex_init(&representer->queue->mutex_queue, NULL) != 0)
  {
    pthread_mutex_destroy(&representer->print_mutex);
    destroy_mutex_cond(&representer->ready_coders_counter_m_c);
    return false;
  }
  return true;
}

bool initialize_representer_struct(t_representer *representer ,int ac, char **av)
{
    representer->config = parser(ac, av);
    representer->coders_counter = 0;
    representer->coders_are_ready = false;
    representer->is_burnouted = false;
    
    init_dongles(representer);
    if (!representer->dongles)
        exit_all("Dongles Allocatoion Error");
    init_coders(representer);
    if (!representer->coders)
        exit_all("Coders Allocatoion Error");
    representer->queue = initializer_queue(representer);
    representer->monitor = monitor_initializer();
    if(!representer->monitor)
        exit_all("Monitor Creation Error");
    representer->manager = manager_initializer();
    if (!representer->manager)
        exit_all("Manager Creation Error");
    return true;
}

t_representer *linker_coders_with_dongles(t_representer *representer)
{
	int i;
	i = 0;
    
    printf("number of coders %d\n", representer->config.number_of_coders);
    while (i < representer->config.number_of_coders)
	{
        // printf("number of coders %d\n", representer->config.number_of_coders);  
        representer->coders[i]->left_dongle = representer->dongles[i];
		representer->coders[i]->right_dongle = representer->dongles[(i + 1) % representer->config.number_of_coders];
		i++;
	}
    return representer;
}
t_queue *initializer_queue(t_representer *representer)
{
    t_queue *queue;

    queue = malloc(sizeof(t_queue) * queue->capacity);
    queue->coders = representer->coders;
    queue->capacity = representer->config.number_of_coders;
    queue->size = 0 ;
    return queue;
}
