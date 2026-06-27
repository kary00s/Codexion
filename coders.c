/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 21:56:43 by kanahiz           #+#    #+#             */
/*   Updated: 2026/06/27 04:28:10 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void free_previous_coders(t_coder **coders, int n) ;

static void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
                                       int number_of_coders);
t_coder **coders_allocater(int number_of_coders);
static void initialize_coders_struct(t_representer *representer);
static bool init_coders_mutexes_conds(t_coder **coders, int number_of_coders);

bool init_coders(t_representer *representer) {
  int number_of_coders;

  number_of_coders = representer->config.number_of_coders;
  representer->coders = coders_allocater(number_of_coders);
  if (!representer->coders)
    return false;
  if (!init_coders_mutexes_conds(representer->coders, number_of_coders)) {
    free_coders(representer);
    return false;
  }
  initialize_coders_struct(representer);
  linker_coders_with_dongles(representer->coders, representer->dongles, number_of_coders);
  return true;
}

t_coder **coders_allocater(int number_of_coders) {
  t_coder **coders_list;
  int i;

  i = 0;
  coders_list = (t_coder **)malloc(sizeof(t_coder *) * number_of_coders);
  if (!coders_list)
    return (NULL);
  while (i < number_of_coders) {
    coders_list[i] = (t_coder *)malloc(sizeof(t_coder));
    if (!coders_list[i]) {
      free_previous_coders(coders_list, i);
      return NULL;
    }
    i++;
  }
  return (coders_list);
}

void destroy_mutex_coders(t_coder **coders, int n) {
  int i;

  i = 0;
  while (i < n) {
    destroy_mutex_cond(&coders[i]->mutex_cond.mutex , &coders[i]->mutex_cond.cond);
    i++;
  }
  free_previous_coders(coders, n);
}

static void initialize_coders_struct(t_representer *representer) {
  int i;
  t_coder **coders;

  i = 0;
  coders = representer->coders;
  while (i < representer->config.number_of_coders) {
    coders[i]->coder_id = i;
    coders[i]->is_burnouted = representer->is_burnouted;
    coders[i]->queue = representer->queue;
    coders[i]->print_mutex = &representer->print_mutex;
    coders[i]->ready_coders_counter = &representer->ready_coders_counter;
    coders[i]->ready_coders_counter_m_c =
        &representer->ready_coders_counter_m_c;
    coders[i]->coder_state = STARTING;
    i++;
  }
}

static bool init_coders_mutexes_conds(t_coder **coders, int number_of_coders) {
  int i;
  i = 0;
  while (i < number_of_coders) {
    if (!init_mutex_cond(&coders[i]->mutex_cond)) {
      free_previous_coders(coders, i);
      return false;
    }
    i++;
  }
  return true;
}

static void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
                                       int number_of_coders) {
  int i;
  i = 0;
  while (i < number_of_coders) {
    coders[i]->left_dongle = dongles[i];
    coders[i]->right_dongle = dongles[(i + 1) % number_of_coders];
    i++;
  }
}

static void free_previous_coders(t_coder **coders, int n) {
  int i;

  i = 0;
  while (i < n) {
    free(coders[i]);
    i++;
  }
  free(coders);
}
