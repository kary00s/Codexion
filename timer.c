/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:57:37 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/29 05:16:20 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"codexion.h"

long timeval_to_ms(struct timeval time)
{
    return (time.tv_sec * 1000L) + (time.tv_usec / 1000L);
}


// purge this shit
//    long time_calculator(struct timeval start)


long get_time_ms()
{
    struct timeval time;
	gettimeofday(&time, NULL);
	return timeval_to_ms(time);
}

int timeval_less(struct timeval a, struct timeval b)
{
    if (a.tv_sec != b.tv_sec)
        return (a.tv_sec < b.tv_sec);
    return (a.tv_usec < b.tv_usec);
}
