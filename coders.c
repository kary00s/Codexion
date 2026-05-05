/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:43 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/05 01:19:02 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"codexion.h"


t_coder **coders_allocater(t_representer *representer)
{
    t_coder **coders_list; 
    int i;

    i = 0;
    coders_list = (t_coder **)malloc(sizeof(t_coder *) * representer->config->number_of_coders);
    if (!coders_list)
        return (NULL);

    while(i < representer->config->number_of_coders)
    {
        coders_list[i] = (t_coder *)malloc (sizeof(t_coder));
        if (!coders_list[i])
        {
            while (i >= 0)
                free(coders_list[i--]);
            return NULL;
        }
        i++;
    }
    return (coders_list);
}


static void free_coders(t_representer *representer)
{
    int i;

    i = 0;
    while((representer->coders[i] != NULL) && ( i <= representer->config->number_of_coders))
    {
        free(representer->coders[i]);
        i++;
    }
    free(representer->coders);
}


static void clear_coders(t_representer *representer)
{
    int i;

    i = 0;
    while (i < representer->config->number_of_coders)
    {
        pthread_mutex_destroy(&representer->coders[i]->mutex);
        pthread_cond_destroy(&representer->coders[i]->cond);
        i++;
    }
    free_coders(representer);
}



bool initialize_coders_struct(t_representer *representer)
{
    t_coder *coder;
    int i;

    i = 0;
    while (i < representer->config->number_of_coders)
    {
        coder->config = &representer->config;
        coder->coder_id = i;
        coder->coders_counter = &representer->coders_counter;
        coder->burnout_mutex = &representer->burnout_mutex;
        coder->is_burnouted = &representer->is_burnouted;
        coder = representer->coders[i];
        coder->coder_state = STARTING;
        pthread_mutex_init(&coder->mutex, NULL);
        pthread_cond_init(&coder->cond, NULL);
    }
    return (true);
}


bool ft_init_coders(t_representer *representer)
{
    t_coder **coders;

    representer->coders  = coders_allocater(representer);
    if(!representer->coders)
        return (free_coders(representer) ,false);

    if(!initialize_coders_struct(representer))
        return(clear_coders(representer), false);

    return (true);
}