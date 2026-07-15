/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutexs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 04:34:32 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 04:34:33 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

bool	init_mutex_cond(t_mutex_cond *mutex_cond)
{
	if (pthread_mutex_init(&mutex_cond->mutex, NULL) != 0)
		return (false);
	if (pthread_cond_init(&mutex_cond->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&mutex_cond->mutex);
		return (false);
	}
	return (true);
}

bool	init_coders_mutexes_conds(t_coder **coders, int number_of_coders)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		if (!init_mutex_cond(&coders[i]->mutex_cond))
		{
			coders_mutexes_destroyer(coders, i);
			return (false);
		}
		i++;
	}
	return (true);
}
