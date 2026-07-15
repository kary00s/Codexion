/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 04:33:45 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 04:33:46 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

bool	controller_creator(t_representer *representer)
{
	if (pthread_create(&representer->controller, NULL, &controller_home,
			representer))
	{
		exit_representation(representer);
		broadcast_coders_to_exit(representer,
				representer->config.number_of_coders);
		representer_mutexes_destroyer(representer);
		clean_initialize_representer_struct(representer);
		return (false);
	}
	return (true);
}

void	controller_joiner(pthread_t *controller)
{
	pthread_join(*controller, NULL);
	return ;
}

void	*controller_home(void *args)
{
	t_representer	*representer;
	t_coder			*coder;

	representer = (t_representer *)args;
	while (is_representation_works_well(&representer->is_burnout_mutex,
			&representer->is_burnout))
	{
		coder = catch_coder(representer);
		if (coder)
		{
			pthread_mutex_lock(&coder->mutex_cond.mutex);
			coder->coder_state = COMPILING;
			pthread_cond_broadcast(&coder->mutex_cond.cond);
			pthread_mutex_unlock(&coder->mutex_cond.mutex);
		}
		else
			usleep(200);
	}
	return (NULL);
}

bool	wait(pthread_mutex_t *mutex, pthread_cond_t *cond, unsigned long time)
{
	t_timespec	time_spec;
	bool		is_ok;

	is_ok = true;
	pthread_mutex_lock(mutex);
	ms_to_timespec(&time_spec, time);
	if (pthread_cond_timedwait(cond, mutex, &time_spec) != ETIMEDOUT)
		is_ok = false;
	pthread_mutex_unlock(mutex);
	return (is_ok);
}

void	linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
		int number_of_coders)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		coders[i]->left_dongle = dongles[i];
		coders[i]->right_dongle = dongles[(i + 1) % number_of_coders];
		i++;
	}
}
