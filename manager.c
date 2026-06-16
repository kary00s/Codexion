/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:06:35 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/16 09:45:38 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"




void manager_creator(t_representer *representer)
{

    // printf("test manager here\n");
    
    if (pthread_create(&representer->manager->manager, NULL, &manager_home, representer))
        exit_all("manager creation error\n");
}

t_manager *manager_initializer(void)
{	t_manager *manager;
	manager = malloc(sizeof(t_manager));
	if(!manager)
		return NULL;
	return manager;
}
void manager_joiner(t_manager *manager)
{
    pthread_join(manager->manager, NULL);
}

void *manager_home(void *args)
{
    t_representer *representer;
    representer = (t_representer *)args;
    t_coder *coder;
    int i = 0;   
       
    while (1)
	{
            
        coder = peek_a_coder(representer);
        if (coder != NULL) {
            pthread_mutex_lock(&coder->mutex);
            *(coder->coder_state) = COMPILING;
            pthread_cond_broadcast(&coder->cond);
            pthread_mutex_unlock(&coder->mutex);
            i++;         
        } 
        else
            usleep(300);  
    }
 
    return NULL;    
}


t_coder *peek_a_coder(t_representer *representer)
{
    t_coder *coder;
    int i = 0;
    if (representer->queue->size == 0)
        return NULL;
    pthread_mutex_lock(&representer->queue->mutex_queue);
    while (i < representer->queue->size)
    {
        
        if (are_dongles_available(representer->coders[i]))
        {
            // pop up the coder from queue
            pthread_mutex_unlock(&representer->queue->mutex_queue);
            return representer->coders[i];
        }
        i++;
    }
    pthread_mutex_unlock(&representer->queue->mutex_queue);
    return coder;
}