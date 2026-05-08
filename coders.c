/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:43 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/09 00:38:44 by kanahiz          ###   ########.fr       */
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



static void clear_coders(t_representer *representer)
{
    int i;

    i = 0;
    while (i < representer->config.number_of_coders)
    {
        pthread_mutex_destroy(&representer->coders[i]->mutex);
        pthread_cond_destroy(&representer->coders[i]->cond);
        i++;
    }
    free_coders(representer);
}



static void initialize_coders_struct(t_representer *representer, t_coder **coder, int counter)
{
    int i;
    i = 0;
    representer->coders_counter = 0;

    while (i < counter)
    {
        coder[i]->coder_id = i;
        representer->coders_counter += 1;
        coder[i]->is_burnouted = representer->is_burnouted;
        if (pthread_mutex_init(&coder[i]->mutex, NULL) != 0)
            clear_coders(representer);
        if (pthread_cond_init(&coder[i]->cond, NULL))
            clear_coders(representer);

        i++;
    }
}


void init_coders(t_representer *representer)
{
    representer->coders  = coders_allocater(representer->coders, representer->config.number_of_coders);
    if(!representer->coders)
        exit_all("from coders allocater");
    
        
    initialize_coders_struct(representer, representer->coders,representer->config.number_of_coders);
    if (!representer->coders)
         clear_coders(representer);

}