/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:53 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/11 22:59:27 by kanahiz          ###   ########.fr       */
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



void init_dongles(t_representer *representer)
{
    
    representer->dongles = dongles_allocater(representer);
    if (!representer->dongles)
        exit_all("from dongles allocation");
        
    representer->dongles = initialize_dongles_struct(representer->dongles, representer->config.number_of_coders);
    if (!representer->dongles)
        free_dongles(representer);


}