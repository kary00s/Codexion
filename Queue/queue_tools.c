/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 05:52:24 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 06:00:54 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static int  smallest_child_edf(t_queue *queue, int i);
static void	shift_queue_down_edf(t_queue *queue, int i);
static void	shift_queue_down_fifo(t_queue *queue, int i);

void	insert_coder_in_queue(t_coder *coder, t_queue *queue)
{
	pthread_mutex_lock(&queue->mutex_queue);
	queue->coders[queue->size] = coder;
	queue->size++;
	pthread_mutex_unlock(&queue->mutex_queue);
}

static void	shift_queue_down_fifo(t_queue *queue, int i)
{
	while (i < queue->size - 1)
	{
		queue->coders[i] = queue->coders[i + 1];
		i++;
	}
	queue->size--;
}

static int  check_child_edf(t_queue *queue, int idx, int smallest,
        unsigned long *smallest_time)
{
    unsigned long   child_time;

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

static int  smallest_child_edf(t_queue *queue, int i)
{
    int             smallest;
    unsigned long   smallest_time;

    smallest = i;
    smallest_time = timeval_to_ms(queue->coders[i]->last_compile);
    smallest = check_child_edf(queue, i * 2 + 1, smallest, &smallest_time);
    smallest = check_child_edf(queue, i * 2 + 2, smallest, &smallest_time);
    return (smallest);
}

static void    shift_queue_down_edf(t_queue *queue, int i)
{
    int    smallest;

    while (1)
    {
        smallest = smallest_child_edf(queue, i);
        if (smallest == i)
            break ;
        swap_coders(&queue->coders[i], &queue->coders[smallest]);
    }
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
