/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 05:52:24 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/16 00:08:02 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	insert_coder_in_queue(t_coder *coder, t_queue *queue)
{
	pthread_mutex_lock(&queue->mutex_queue);
	queue->coders[queue->size] = coder;
	queue->size++;
	pthread_mutex_unlock(&queue->mutex_queue);
}

bool	pop_coder_from_queue(t_representer *representer, int i)
{
	t_queue	*queue;

	queue = representer->queue;
	if (representer->queue->coders[i] == NULL)
		return (false);
	if (representer->config.scheduler == FIFO)
		shift_queue_down_fifo(queue, i);
	else if (representer->config.scheduler == EDF)
	{
		swap_coders(&representer->queue->coders[0],
			&representer->queue->coders[i]);
		swap_coders(&queue->coders[0], &queue->coders[queue->size - 1]);
		queue->size--;
		shift_queue_down_edf(queue, 0);
	}
	return (true);
}
