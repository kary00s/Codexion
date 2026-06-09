/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/09 23:35:52 by kanahiz          ###   ########.fr       */
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
    init_queue(representer);
    
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

void *manager(void *args)
{
    t_representer *representer;
    representer = (t_representer *)args;
    t_coder *coder;
    
    while (1)
    {
        coder = ft_peeK();
        if (coder != NULL) {
            // change state of dongles left and right to locked
            pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
            coder->left_dongle->is_available = false;
            pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
            
            pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
            coder->right_dongle->is_available = false;
            pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
            
            pthread_mutex_lock(&coder->mutex);
            coder->coder_state = COMPILING;
            pthread_cond_broadcast(&coder->cond);
            pthread_mutex_unlock(&coder->mutex);
            
        }
        usleep(300);
    }
    
}