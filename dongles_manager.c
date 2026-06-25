/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   holder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:07:49 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/10 15:07:51 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "codexion.h"

static void drop_dongle(t_dongle *dongle);
static void hold_dongle(t_dongle *dongle);


void hold_both_dongles(t_coder *coder)
{
	hold_dongle(coder->left_dongle);	
	hold_dongle(coder->right_dongle);
    
    
    pthread_mutex_lock(coder->print_mutex);
    printf("time %d has taken dongle\n", coder->coder_id);
    printf("time %d has taken dongle\n", coder->coder_id);
    pthread_mutex_lock(coder->print_mutex);
}

void drop_both_dongles(t_coder *coder)
{
	drop_dongle(coder->right_dongle);
	drop_dongle(coder->left_dongle);
}

static void hold_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_mutex);
    dongle->is_available = false;
    pthread_mutex_unlock(&dongle->dongle_mutex);
}


static void drop_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_mutex);
    dongle->is_available = true;
    pthread_mutex_unlock(&dongle->dongle_mutex);
}
