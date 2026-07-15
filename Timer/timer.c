/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:57:37 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 07:00:00 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

unsigned long	timeval_to_ms(struct timeval time)
{
	return ((time.tv_sec * 1000L) + (time.tv_usec / 1000L));
}

unsigned long	get_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (timeval_to_ms(time));
}

unsigned long	time_dongle_get_cold(unsigned long time_cooldown)
{
	unsigned long	right_now;

	right_now = get_time_ms();
	return (right_now + time_cooldown);
}
