/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 04:32:54 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 04:55:38 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

void	free_dongles(t_representer *representer)
{
	int	i;

	i = 0;
	while (i < representer->config.number_of_coders)
		free(representer->dongles[i++]);
	free(representer->dongles);
}

void	free_coders(t_coder **coders, int n)
{
	int	i;

	i = 0;
	while ((i < n))
	{
		free(coders[i]);
		i++;
	}
	free(coders);
}

void	clean_initialize_representer_struct(t_representer *representer)
{
	clean_coders(representer);
	clean_queue(representer);
	clean_dongles(representer);
}
