/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/11 15:29:56 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void monitor_creator(t_representer *representer)
{
	if (pthread_create(&(representer->monitor->monitor), NULL, monitor_home, representer))
    	exit_all("Error: monitor thread failed\n");
}

void monitor_joiner(t_monitor *monitor)
{
	pthread_join(monitor->monitor, NULL);
}

void *monitor_home(void *args)
{
	t_representer *representer;
	representer = (t_representer *)args;
	int waiting_time;
	t_coder *coder;
	int i = 0;
	while (1)
	{
	    if (i == representer->queue->size)
            i = 0;
		coder = peek_a_coder(representer->queue, i);
		waiting_time = time_calculator(coder, coder->last_compile); 
		
		// send a segnal to manager to exit 
		if(waiting_time > representer->config.time_to_burnout)
			representer->is_burnouted =  true;
		usleep(100);
		i++;
	}
	
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