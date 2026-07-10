/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:57:37 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/02 13:57:28 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"codexion.h"

unsigned long timeval_to_ms(struct timeval time)
{
    return (time.tv_sec * 1000L) + (time.tv_usec / 1000L);
}


unsigned long get_time_ms()
{
    struct timeval time;
	gettimeofday(&time, NULL);
	return timeval_to_ms(time);
}

unsigned long time_dongle_get_cold(unsigned long time_cooldown)
{
    unsigned long right_now;
    right_now = get_time_ms();
    return right_now + time_cooldown;
} 



void	ms_to_timespec(t_timespec *timespec, unsigned long time_ms)
{
	timespec->tv_sec = time_ms / 1000;
	timespec->tv_nsec = (time_ms % 1000) * 1000000L;
}

long time_elapsed_until_now(struct timeval elapsed_time)
{
    long  time_taken;
    long rn = get_time_ms();
    time_taken = timeval_to_ms(elapsed_time);
    time_taken = rn - time_taken;
    return time_taken;
}

void register_time(struct timeval *time, pthread_mutex_t *mutex)
{
  pthread_mutex_lock(mutex);
  gettimeofday(time, NULL);
  pthread_mutex_unlock(mutex);
}