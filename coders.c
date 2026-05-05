/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: karim <karim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:43 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/05 10:36:00 by karim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"codexion.h"


static t_coder **coders_allocater(t_coder **coder, int number_of_coders)
{
    t_coder **coders_list; 
    int i;

    i = 0;
    coders_list = (t_coder **)malloc(sizeof(t_coder *) * number_of_coders);
    if (!coders_list)
        return (NULL);

    while(i < number_of_coders)
    {
        coders_list[i] = (t_coder *)malloc (sizeof(t_coder));
        if (!coders_list[i])
        {
            while (i >= 0)
                free(coders_list[i--]);
            free(coders_list);
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
        free(representer->coders[i++]);
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



static void initialize_coders_struct(t_representer *representer,  t_coder **coder, int counter)
{
    int i;

    i = 0;
    while (i < counter)
    {
        coder[i]->config = representer->config;
        coder[i]->coder_id = i;
        
        coder[i]->coders_counter = representer->coders_counter;
        coder[i]->burnout_mutex = &representer->burnout_mutex;
        coder[i]->is_burnouted = representer->is_burnouted;
        // coder[i] = representer->coders[i];
        // coder[i]->coder_state = STARTING;
        pthread_mutex_init(&coder[i]->mutex, NULL);
        pthread_cond_init(&coder[i]->cond, NULL);
        i++;
    }

}


t_coder **init_coders(t_representer *representer)
{
    t_coder **coders;
    coders  = coders_allocater(coders, representer->config->number_of_coders);
    if(!coders)
        return (NULL);
    
    initialize_coders_struct(representer, coders, representer->config->number_of_coders);
        //fix it later
        // return(clear_coders(representer), NULL);

    return (coders);
}