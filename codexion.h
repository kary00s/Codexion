/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanahiz <kanahiz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 06:42:08 by kanahiz           #+#    #+#             */
/*   Updated: 2026/07/15 07:22:22 by kanahiz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# define LONG_MAX 9223372036854775807LL

# include <bits/types/struct_timeval.h>
# include <errno.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_controller		t_controller;

typedef struct timespec			t_timespec;
typedef struct timeval			t_timeval;
typedef struct s_dongle			t_dongle;
typedef struct s_coder			t_coder;
typedef struct s_representer	t_representer;
typedef struct s_config			t_config;

typedef struct s_mutex_cond
{
	pthread_mutex_t				mutex;
	pthread_cond_t				cond;
}								t_mutex_cond;

typedef struct s_queue
{
	t_coder						**coders;
	int							size;
	int							capacity;
	pthread_mutex_t				mutex_queue;
}								t_queue;

typedef struct s_dongle
{
	int							dongle_id;
	struct timeval				last_reste;
	bool						is_available;
	bool						is_cold;
	t_timespec					time_reste;
	pthread_mutex_t				dongle_mutex;
}								t_dongle;

typedef enum enum_scheduer
{
	FIFO,
	EDF
}								t_scheduler;

typedef enum e_coder_state
{
	START,
	WAIT,
	COMPILING,
	DEBUGING,
	REFACTORING,
	EXIT,
}								t_coder_state;

typedef struct s_config
{
	long						number_of_coders;
	long						time_to_debug;
	long						time_to_burnout;
	long						time_to_compile;
	long						time_to_refactor;
	long						number_of_compiles_required;
	long						dongle_cooldown;
	t_scheduler					scheduler;
}								t_config;

typedef struct s_coder
{
	bool						*is_burnout;
	t_config					*config;
	t_dongle					*right_dongle;
	t_dongle					*left_dongle;
	t_queue						*queue;
	pthread_t					thread;
	t_coder_state				coder_state;
	t_mutex_cond				mutex_cond;
	t_mutex_cond				*ready_coders_m_c;
	pthread_mutex_t				*is_burnout_mutex;
	pthread_mutex_t				*print_mutex;
	int							coder_id;
	int							*ready_coders_counter;
	int							numbers_compilation;
	int							*finished_coders;
	pthread_mutex_t				*finished_coders_mutex;
	struct timeval				*begining_time;
	struct timeval				last_compile;
}								t_coder;

typedef struct s_representer
{
	t_config					config;
	t_dongle					**dongles;
	t_coder						**coders;
	t_queue						*queue;
	bool						is_burnout;
	t_mutex_cond				ready_coders_m_c;
	pthread_mutex_t				print_mutex;
	pthread_mutex_t				is_burnout_mutex;
	pthread_mutex_t				finished_coders_mutex;
	pthread_t					monitor;
	pthread_t					controller;
	int							ready_coders_counter;
	int							finshed_coders;
	struct timeval				begining_time;
}								t_representer;

void			free_dongles(t_representer *representer);
void			free_coders(t_coder **coders, int n);
void			clean_initialize_representer_struct(t_representer *representer);
void			clean_dongles(t_representer *representer);
void			clean_coders(t_representer *representer);

bool			init_coders(t_representer *representer);
t_coder			**coders_allocater(int number_of_coders);
void			coders_joiner(t_representer *representer);
bool			coders_creator(t_representer *representer);

bool			coder_waiting_dongles(t_coder *coder);
t_coder			*catch_coder(t_representer *representer);
bool			are_one_of_coders_burnout(t_representer *representer);
bool			is_coder_burnouted(t_coder *coder);
void			swap_coders(t_coder **parent_coder, t_coder **child_coder);
void			allow_coders_to_start(t_representer *representer);
void			broadcast_coders_to_exit(t_representer *representer, int i);
bool			wait_for_coders_to_start(t_representer *representer);
bool			controller_creator(t_representer *representer);
void			*controller_home(void *args);
void			controller_joiner(pthread_t *controller);
void			linker_coders_with_dongles(t_coder **coders, t_dongle **dongles,
					int number_of_coders);
bool			wait(pthread_mutex_t *mutex, pthread_cond_t *cond,
					unsigned long time);
bool			is_represontation_done(t_representer *representer);
void			exit_representation(t_representer *representer);
bool			is_representation_works_well(pthread_mutex_t *is_burnout_mutex,
					bool *is_burnout);
bool			wait_for_representation_to_start(t_coder *coder);

void			*routine_all_the_coders(void *arg);
void			add_coder_to_finished_coders(t_coder *coder);
void			print_action(t_coder *coder);

bool			compiling(t_coder *coder);
bool			debuging(t_coder *coder);
bool			refactoring(t_coder *coder);

bool			init_dongles(t_representer *representer);

void			drop_both_dongles(t_coder *coder);
void			make_dongles_unavailable(t_dongle *dongle);
bool			are_dongles_available(t_coder *coder);
bool			wait_dongles_to_cold(t_coder *coder, long cooldown_time);

bool			is_dongle_ready(t_dongle *dongle);
bool			check_dongles_coldness(t_coder *coder);

bool			initialize_representer_struct(t_representer *representer,
					int ac, char **av);
bool			init_representer_mutexs_conds(t_representer *representer);

bool			monitor_creator(t_representer *representer);
void			monitor_joiner(pthread_t *monitor);
void			*monitor_home(void *args);

bool			init_mutex_cond(t_mutex_cond *mutex_cond);
bool			init_coders_mutexes_conds(t_coder **coders,
					int number_of_coders);

void			dongles_mutexes_destroyer(t_dongle **dongles, int counter);
void			representer_mutexes_destroyer(t_representer *representer);
void			destroy_mutex_cond(t_mutex_cond *mutex_cond);
void			coders_mutexes_destroyer(t_coder **coders, int n);

int				ft_strcmp(char *s1, char *s2);
bool			parser(int ac, char **args, t_representer *representer);

bool			pop_coder_from_queue(t_representer *representer, int i);
void			insert_coder_in_queue(t_coder *coder, t_queue *queue);
bool			init_queue(t_representer *representer);
bool			init_queue_mutexs_conds(t_representer *representer);
void			clean_queue(t_representer *representer);

unsigned long	timeval_to_ms(struct timeval time);
unsigned long	get_time_ms(void);
void			ms_to_timespec(t_timespec *timespec, unsigned long time_ms);
long			time_elapsed_until_now(struct timeval elapsed_time);
void			register_time(struct timeval *time, pthread_mutex_t *mutex);

#endif
