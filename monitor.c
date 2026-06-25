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

void monitor_creator(t_representer *representer)
{
	if (pthread_create(&representer->monitor->monitor, NULL, &monitor_home, representer))
		exit_all("error monitor creation\n");
}

t_monitor *monitor_initializer(void)
{	t_monitor *monitor;
	monitor = malloc(sizeof(t_monitor));
	if(!monitor)
		return NULL;
	return monitor;
}

void monitor_joiner(t_monitor *monitor)
{
	pthread_join(monitor->monitor, NULL);
}

void *monitor_home(void *args)
{
	t_representer *representer;
	representer = (t_representer *)args;
	// int waiting_time;
	t_coder *coder;
	// int i = 0;
	// if(representer->coders_are_ready == true)
	// {
	// 	while (1)
	// 	{
	// 		printf("coder id\n");
	// 		if (i >= representer->queue->size)
	// 		i = 0;
	// 		coder = peek_a_coder(representer);
	// 		if(coder == NULL)
	// 			continue;
	// 		
	// 		waiting_time = time_calculator(coder, coder->last_compile); 
	// 		if(waiting_time > representer->config.time_to_burnout)
	// 			representer->is_burnouted =  true;
	// 		// send a segnal to manager to exit 
	// 		usleep(100);
	// 		i++;
	// 	}
	// }
	return NULL;
	
}

