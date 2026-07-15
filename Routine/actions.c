/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 05:46:20 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 06:55:12 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

static void	change_codere_state(t_coder *coder, t_coder_state state);
static bool	action_simulator(t_coder *coder, t_coder_state state);

bool	compiling(t_coder *coder)
{
	insert_coder_in_queue(coder, coder->queue);
	if (!coder_waiting_dongles(coder))
		return (false);
	if (!check_dongles_coldness(coder))
		return (false);
	print_action(coder);
	register_time(&coder->last_compile, &coder->mutex_cond.mutex);
	add_coder_to_finished_coders(coder);
	if (!action_simulator(coder, coder->coder_state))
		return (false);
	drop_both_dongles(coder);
	change_codere_state(coder, DEBUGING);
	return (true);
}

bool	debuging(t_coder *coder)
{
	print_action(coder);
	if (!action_simulator(coder, coder->coder_state))
		return (false);
	change_codere_state(coder, REFACTORING);
	return (is_representation_works_well(coder->is_burnout_mutex,
			coder->is_burnout));
}

bool	refactoring(t_coder *coder)
{
	print_action(coder);
	if (!action_simulator(coder, coder->coder_state))
		return (false);
	change_codere_state(coder, WAIT);
	return (is_representation_works_well(
      coder->is_burnout_mutex,
      coder->is_burnout));
}

static bool	action_simulator(t_coder *coder, t_coder_state state)
{
	unsigned long	time_action;
	unsigned long	right_now;

	right_now = get_time_ms();
	time_action = right_now;
	if (state == COMPILING)
		time_action += coder->config->time_to_compile;
	else if (state == DEBUGING)
		time_action += coder->config->time_to_debug;
	else if (state == REFACTORING)
		time_action += coder->config->time_to_refactor;
	if (!wait(&coder->mutex_cond.mutex, &coder->mutex_cond.cond, time_action))
		return (is_representation_works_well(coder->is_burnout_mutex,
												coder->is_burnout));
	return (true);
}

static void	change_codere_state(t_coder *coder, t_coder_state state)
{
	pthread_mutex_lock(&coder->mutex_cond.mutex);
	coder->coder_state = state;
	pthread_mutex_unlock(&coder->mutex_cond.mutex);
}