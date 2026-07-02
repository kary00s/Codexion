/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:57:37 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/02 06:01:38 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"codexion.h"

long timeval_to_ms(struct timeval time)
{
    return (time.tv_sec * 1000L) + (time.tv_usec / 1000L);
}


long get_time_ms()
{
    struct timeval time;
	gettimeofday(&time, NULL);
	return timeval_to_ms(time);
}

long time_dongle_get_cold(long time_cooldown)
{
    long right_now;
    right_now = get_time_ms();
    return right_now + time_cooldown;
} 

int timeval_less(struct timeval a, struct timeval b)
{
    if (a.tv_sec != b.tv_sec)
        return (a.tv_sec < b.tv_sec);
    return (a.tv_usec < b.tv_usec);
}