/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:07:49 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/28 03:56:44 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"codexion.h"
static void drop_dongle(t_dongle *dongle);
static void hold_dongle(t_dongle *dongle);


bool hold_both_dongles(t_coder *coder)
{
    time_t right_now;
    right_now = get_time_ms();
    if(    coder->left_dongle->is_available
        && coder->right_dongle->is_available
        && coder->left_dongle->time_cooldown < right_now
        && coder->right_dongle->time_cooldown < right_now )
    {
	    hold_dongle(coder->left_dongle);	
	    hold_dongle(coder->right_dongle);
        
    
        pthread_mutex_lock(coder->print_mutex);
        printf("%ld %d has taken dongle\n",
        time_calculator(coder->last_compile),
        coder->coder_id);
        pthread_mutex_unlock(coder->print_mutex);
        
        pthread_mutex_lock(coder->print_mutex);
        printf("%ld %d has taken dongle\n",
        time_calculator(coder->last_compile),
            coder->coder_id);
        pthread_mutex_unlock(coder->print_mutex);
        return true;
    }

    return false;
}

void drop_both_dongles(t_coder *coder)
{
    time_t right_now;
	
    drop_dongle(coder->right_dongle);
	drop_dongle(coder->left_dongle);
    
    pthread_mutex_lock(&coder->left_dongle->dongle_m_c.mutex);
	pthread_mutex_lock(&coder->right_dongle->dongle_m_c.mutex);
    
    right_now = get_time_ms();
    coder->left_dongle->time_cooldown = right_now + coder->config->dongle_cooldown;
	coder->right_dongle->time_cooldown = right_now + coder->config->dongle_cooldown;

    pthread_mutex_unlock(&coder->left_dongle->dongle_m_c.mutex);
	pthread_mutex_unlock(&coder->right_dongle->dongle_m_c.mutex);
	
}

static void hold_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = false;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}


static void drop_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = true;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}
