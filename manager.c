/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:06:35 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/12 23:29:50 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"




void manager_creator(t_representer *representer)
{
        
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
        // printf("======hna==\n");
        coder = peek_a_coder(representer->queue, i);
        if (coder != NULL && coder->left_dongle->is_available == true && coder->right_dongle->is_available == true)
        {
            pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
            coder->left_dongle->is_available = false;
            pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
            
            pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
            coder->right_dongle->is_available = false;
            pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
            
            pthread_mutex_lock(&coder->mutex);
            *(coder->coder_state) = COMPILING;
            pthread_cond_broadcast(&coder->cond);
            pthread_mutex_unlock(&coder->mutex);
        }
        if (i == representer->queue->size)
        i = 0;
        usleep(300);
        i++;
    }
    return NULL;    
}


t_coder *peek_a_coder(t_queue *queue ,int index)
{
    return queue->coders[index];
}