/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:53 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/15 01:05:31 by kanahiz          ###   ########.fr       */
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
        dongles[i]->dongle_id = i;

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

static bool is_dongle_available(t_dongle *dongle) {
    bool is_available;
    is_available = false;
    pthread_mutex_lock(&dongle->dongle_mutex);
    if (dongle->is_available)
        is_available = true;
    pthread_mutex_unlock(&dongle->dongle_mutex);
    return is_available;
}

static void make_dongles_unavailable(t_coder *coder)
{
    pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
    coder->left_dongle->is_available = false;
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);

    pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
    coder->right_dongle->is_available = false;
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}

bool are_dongles_available(t_coder *coder)
{
    if (is_dongle_available(coder->left_dongle) && is_dongle_available(coder->right_dongle))
    {
        make_dongles_unavailable(coder);
        return true;    
    }
    return false;   
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