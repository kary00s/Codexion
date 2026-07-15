/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner_sec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 04:53:20 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 05:00:26 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	clean_queue(t_representer *representer)
{
	pthread_mutex_destroy(&representer->queue->mutex_queue);
	free(representer->queue->coders);
	free(representer->queue);
}

void	clean_coders(t_representer *representer)
{
	long	number_of_coders;

	number_of_coders = representer->config.number_of_coders;
	coders_mutexes_destroyer(representer->coders, number_of_coders);
	free_coders(representer->coders, number_of_coders);
}

void	clean_dongles(t_representer *representer)
{
	long	number_of_coders;

	number_of_coders = representer->config.number_of_coders;
	dongles_mutexes_destroyer(representer->dongles, number_of_coders);
	free_dongles(representer);
}
