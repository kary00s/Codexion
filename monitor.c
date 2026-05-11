/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/11 09:48:25 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void initialize_representer_struct(t_representer *representer ,int ac, char **av)
{
    representer->config = parser(ac, av);
    init_dongles(representer);
    if (!representer->dongles)
        exit_all("Dongles Allocatoion Error");
    init_coders(representer);
    if (!representer->coders)
        exit_all("Coders Allocatoion Error");
    representer->coders_are_ready = false;

    if(pthread_mutex_init(&representer->burnout_mutex, NULL))
         pthread_mutex_destroy(&representer->burnout_mutex);
    if(pthread_mutex_init(&representer->mutex, NULL))
        pthread_mutex_destroy(&representer->mutex);
    if(pthread_cond_init(&representer->cond, NULL))
        pthread_cond_destroy(&representer->cond);
        representer->is_burnouted = false; 
    representer->coders_counter = 0;
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