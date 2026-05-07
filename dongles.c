/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:53 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/07 14:51:02 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_dongle **dongles_allocater(t_representer *representer)
{
    t_dongle **dongles_list;
    int i;
    i = 0;

    dongles_list = (t_dongle **)malloc(sizeof(t_dongle *) * representer->config.number_of_coders);
    if (!dongles_list)
        return (NULL);

    while(i < representer->config.number_of_coders)
    {
        dongles_list[i] = (t_dongle *)malloc (sizeof(t_dongle));
        if (!dongles_list[i])
        {
            while (i >= 0)
                free(dongles_list[i--]);
            free(dongles_list);
            return NULL;
        }
        i++;
    }
    return (dongles_list);
}

static t_dongle **initialize_dongles_struct(t_dongle **dongles, int counter) {
    int i;
    
    i = 0;
    while (i < counter)
    {
        dongles[i]->is_available = true;

        if (pthread_mutex_init(&dongles[i]->dongle_mutex, NULL))
        {
            dongles_destroyer(dongles, i);
            return (NULL);
        }
        if (pthread_cond_init(&dongles[i]->dongle_cond, NULL))
        {
            dongles_destroyer(dongles, i);
            return (NULL);    
        }
        i++;
    }
    return (dongles);
}

void dongles_destroyer(t_dongle **dongles, int counter)
{
    int i;
    i = 0;
    while(i < counter)
    {
        pthread_mutex_destroy(&dongles[i]->dongle_mutex);       
        pthread_cond_destroy(&dongles[i]->dongle_cond);    
        i++;
    }
}

static void free_dongles(t_representer *representer)
{
    int i;

    i = 0;
    while((representer->dongles[i] != NULL) && ( i <= representer->config.number_of_coders))
        free(representer->dongles[i++]);
    free(representer->dongles);
}

t_dongle **init_dongles(t_representer *representer)
{
    t_dongle **dongles;
    dongles = dongles_allocater(representer);
    if (!dongles)
        return (NULL);
        
    dongles = initialize_dongles_struct(dongles, representer->config.number_of_coders);
    if (!dongles)
        return (free_dongles(representer), NULL);

    
    return dongles;
}