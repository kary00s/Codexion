/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karim <karim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:23:38 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/05 10:29:13 by karim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool initialize_representer_struct(t_representer *representer ,int ac, char **av)
{
    representer = (t_representer *)malloc(sizeof(t_representer));
    representer->config = parser( ac, av);
    representer->dongles =  init_dongles(representer);
    if (!representer->dongles)
        return (false);
    representer->coders = init_coders(representer);
    if (!representer->coders)
        return (false);
    printf("========\n");
        
    if(pthread_mutex_init(&representer->burnout_mutex, NULL));
        return (pthread_mutex_destroy(&representer->burnout_mutex), false);

    if(pthread_mutex_init(&representer->mutex, NULL));
        return (pthread_mutex_destroy(&representer->mutex), false);
    if(pthread_cond_init(&representer->cond, NULL));
        return (pthread_cond_destroy(&representer->cond), false);
        representer->is_burnouted = false; 
    representer->coders_counter = 0;
    return(true);
}