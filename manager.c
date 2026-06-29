/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manager.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:06:35 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/29 06:57:49 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void *manager_home(void *args);
bool manager_creator(t_representer *representer) {

  if (pthread_create(&representer->manager, NULL, &manager_home, representer))
    return false;
  return true;
}

void manager_joiner(pthread_t *manager) {
  pthread_join(*manager, NULL);
  return;
}

void *manager_home(void *args) {
  t_representer *representer;
  representer = (t_representer *)args;
  t_coder *coder;

  while (1) 
  {
    coder = catch_coder(representer);
    if (coder != NULL && coder->coder_state == WAITING) { 
      pthread_mutex_lock(&coder->mutex_cond.mutex);
      coder->coder_state = COMPILING;
      pthread_cond_broadcast(&coder->mutex_cond.cond);
      pthread_mutex_unlock(&coder->mutex_cond.mutex);
    }
    else
      usleep(10);
  }
  return NULL;
}

t_coder *catch_coder(t_representer *representer) {
  t_coder *coder;
  int i = 0;
  coder = NULL;
  
  pthread_mutex_lock(&representer->queue->mutex_queue);
  while (i < representer->queue->size)
  {
    if (are_dongles_available(representer->coders[i])) 
    {
      coder = representer->coders[i];
      pop_coder_from_queue(representer, i);
      pthread_mutex_unlock(&representer->queue->mutex_queue);
      return coder;
    }
    i++;
  }
  pthread_mutex_unlock(&representer->queue->mutex_queue);
  return coder;
}
