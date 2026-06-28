/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:57:37 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/28 05:22:33 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"codexion.h"



long timeval_to_ms(struct timeval t)
{
    return (t.tv_sec * 1000) + (t.tv_usec / 1000);
}

long time_calculator(struct timeval start)
{
    return (get_time_ms() - timeval_to_ms(start));
}


long get_time_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000) ;
}

int timeval_less(struct timeval a, struct timeval b)
{
    if (a.tv_sec != b.tv_sec)
        return (a.tv_sec < b.tv_sec);
    return (a.tv_usec < b.tv_usec);
}