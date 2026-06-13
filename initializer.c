/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 14:52:28 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/13 00:18:49 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void initialize_representer_struct(t_representer *representer ,int ac, char **av)
{
    representer->coders_counter = 0;
    representer->config = parser(ac, av);
    init_dongles(representer);
    if (!representer->dongles)
        exit_all("Dongles Allocatoion Error");
    init_coders(representer);
    if (!representer->coders)
        exit_all("Coders Allocatoion Error");
    representer->coders_are_ready = false;
    representer->is_burnouted = false;
    representer->queue = initializer_queue(representer);
    
    if(pthread_mutex_init(&representer->burnout_mutex, NULL))
         pthread_mutex_destroy(&representer->burnout_mutex);
    if(pthread_mutex_init(&representer->mutex, NULL))
        pthread_mutex_destroy(&representer->mutex);
    if(pthread_cond_init(&representer->cond, NULL))
        pthread_cond_destroy(&representer->cond);
    representer->monitor = monitor_initializer();
    if(!representer->monitor)
        exit_all("Monitor Creation Error");
    representer->manager = manager_initializer();
    if (!representer->manager)
        exit_all("Manager Creation Error");
}
void linker_coders_with_dongles(t_representer *representer)
{
	int i;
	i = 0;   
	while (i < representer->config.number_of_coders)
	{
		representer->coders[i]->left_dongle = representer->dongles[i];
		representer->coders[i]->right_dongle = representer->dongles[(i + 1) % representer->config.number_of_coders];
		i++;
	}
}
void *initializer_queue(t_representer *representer)
{
    t_queue *queue;
    queue->capacity = representer->config.number_of_coders;
    queue = malloc(sizeof(queue) * queue->capacity);
    pthread_mutex_init(&queue->mutex_queue, NULL);
    return queue;
}