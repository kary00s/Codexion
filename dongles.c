/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:53 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/05 01:17:31 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_dongle **dongles_allocater(int dongles_num)
{
    t_dongle **dongles_list;
    int i;
    i = 0;

    dongles_list = (t_dongle **)malloc(sizeof(t_dongle *) * dongles_num);
    if (!dongles_list)
        return (NULL);

    while(i < dongles_num)
    {
        dongles_list[i] = (t_dongle *)malloc (sizeof(t_dongle));
        if (!dongles_list[i])
        {
            while (i >= 0)
                free(dongles_list[i--]);
            return NULL;
        }
        i++;
    }
    return (dongles_list);
}

bool initialize_dongles_struct(t_representer *representer) {
    int i;
    t_dongle **dongles;
    
    i = 0;
    dongles = representer->dongles;
    while (i < representer->config->number_of_coders) {
        dongles[i]->is_available = true;
        if (pthread_mutex_init(&dongles[i]->mutex, NULL)) {
            while (i > 0) {
                pthread_mutex_destroy(&dongles[i - 1]->mutex);
                i--;
            }
            return (false);
        }
        i++;
    }
    return true;
}


void dongles_cleaner(t_representer *representer) {
    int i;

    i = 0;
    while (i < representer->config.number_of_coders) {
        pthread_mutex_destroy(&representer->dongles[i]->mutex);
        free(representer->dongles[i]);
        i++;
    }
    free(representer->dongles);
}

bool init_dongles(t_representer *representer) {
    int i;
    t_dongle **dongles;
    int j;
    
    i = 0;
    j = 0;
    dongles = ft_allocate_dongles(representer->config.number_of_coders);
    if (!dongles)
        return false;
    if (!ft_init_dongles_initial_state(representer))
        return (ft_clean_dongles(representer), false);
    representer->dongles = dongles;
    return true;
}