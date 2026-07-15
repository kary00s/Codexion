/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_sec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 06:59:40 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 06:59:57 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	ms_to_timespec(t_timespec *timespec, unsigned long time_ms)
{
	timespec->tv_sec = time_ms / 1000;
	timespec->tv_nsec = (time_ms % 1000) * 1000000L;
}

long	time_elapsed_until_now(struct timeval elapsed_time)
{
	long	time_taken;
	long	rn;

	rn = get_time_ms();
	time_taken = timeval_to_ms(elapsed_time);
	time_taken = rn - time_taken;
	return (time_taken);
}

void	register_time(struct timeval *time, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	gettimeofday(time, NULL);
	pthread_mutex_unlock(mutex);
}
