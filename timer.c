/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:57:37 by kanahiz           #+#    #+#             */
/*   Updated: 2026/05/09 00:40:16 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"codexion.h"

long get_time_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000) ;
}

long time_to_compile(t_coder *coder, long start_time)
{
	usleep(1000000);
	long now = get_time_ms();
	return now - start_time;
}
