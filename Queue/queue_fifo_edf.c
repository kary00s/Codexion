/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_fifo_edf.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 00:03:08 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/16 00:11:01 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	shift_queue_down_fifo(t_queue *queue, int i)
{
	while (i < queue->size - 1)
	{
		queue->coders[i] = queue->coders[i + 1];
		i++;
	}
	queue->size--;
}

int	check_child_edf(t_queue *queue, int idx, int smallest,
		unsigned long *smallest_time)
{
	unsigned long	child_time;

	if (idx >= queue->size)
		return (smallest);
	child_time = timeval_to_ms(queue->coders[idx]->last_compile);
	if (child_time < *smallest_time)
	{
		*smallest_time = child_time;
		return (idx);
	}
	return (smallest);
}

int	smallest_child_edf(t_queue *queue, int i)
{
	int				smallest;
	unsigned long	smallest_time;

	smallest = i;
	smallest_time = timeval_to_ms(queue->coders[i]->last_compile);
	smallest = check_child_edf(queue, i * 2 + 1, smallest, &smallest_time);
	smallest = check_child_edf(queue, i * 2 + 2, smallest, &smallest_time);
	return (smallest);
}

void	shift_queue_down_edf(t_queue *queue, int i)
{
	int	smallest;

	while (1)
	{
		smallest = smallest_child_edf(queue, i);
		if (smallest == i)
			break ;
		swap_coders(&queue->coders[i], &queue->coders[smallest]);
	}
}
