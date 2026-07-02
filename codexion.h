#ifndef CODEXION_H
#define CODEXION_H

#include <bits/types/struct_timeval.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
typedef struct s_controller t_controller;

typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
typedef struct s_representer  t_representer;
typedef struct s_config t_config;

typedef struct s_mutex_cond {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} t_mutex_cond;

// QUEUE structs :
typedef struct s_queue
{
    t_coder         **coders;
    int             size;
    int             capacity;
    pthread_mutex_t mutex_queue;
} t_queue;

// DONGLES structs :
typedef struct s_dongle
{
    int           dongle_id;
    long          last_reste;
    bool          is_available;
    bool          is_cold;
    struct timeval cooldown_timer;
   	t_mutex_cond   dongle_m_c;
} t_dongle;

// SCHEDULER enum :
typedef enum enum_scheduer
{
    FIFO,
    EDF
}   t_scheduler;

// CODER_STATE enum :
typedef enum e_coder_state
{
    STARTING,
    WAITING,
    COMPILING,
    DEBUGING,
    REFACTORING,
    FINISHING
}   t_coder_state;

// REPRESENTER structs :
typedef struct s_config
{
    int number_of_coders;
    int time_to_debug;
    int time_to_burnout;
    int time_to_compile;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    t_scheduler scheduler;
} t_config; 


typedef struct s_coder
{
	int coder_id;
	int coders_counter;
	pthread_mutex_t burnout_mutex;
	pthread_t thread;
	time_t				access;	
	struct timeval last_compile;
  t_queue *queue;
  t_config *config;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	t_coder_state coder_state;
  t_mutex_cond mutex_cond;
	bool is_burnouted;
  pthread_mutex_t *print_mutex;
  int *ready_coders_counter;
  t_mutex_cond *ready_coders_counter_m_c;
}	t_coder;


typedef struct s_representer
{
    int coders_counter;
    t_config config;
    t_dongle **dongles;
    t_coder **coders;
    bool coders_are_ready;
    bool is_burnouted;
    struct timeval start_time;
    t_queue *queue;
    t_mutex_cond m_c;
    pthread_mutex_t print_mutex;
    int ready_coders_counter;
    t_mutex_cond ready_coders_counter_m_c;
    pthread_t monitor;
    pthread_t controller;
    pthread_cond_t cond_monitor;    
    pthread_mutex_t mutex_monitor; 
} t_representer;

// ============= Cleaner ====================>
void free_dongles(t_representer *representer);
void free_coders(t_representer *representer) ;
void clean_initialize_representer_struct(t_representer *representer);


// ============= Coders ====================>
void free_previous_coders(t_coder **coders, int n);
bool coders_creator(t_representer *representer); 
bool init_coders(t_representer *representer);
t_coder **coders_allocater(int number_of_coders);
void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles, int number_of_coders);

// ============= Controller ====================>
bool controller_creator(t_representer *representer);
void *controller_home(void *args) ;
void controller_joiner(pthread_t *controller);
t_coder *catch_coder(t_representer *representer);

void *routine_all_the_coders(void *arg);
bool wait_for_simulation_to_start(t_coder *coder);
void coders_joiner(t_representer *representer);

// ============= Dongles ====================>
bool hold_both_dongles(t_coder *coder);
bool drop_both_dongles(t_coder *coder);
bool are_dongles_available(t_coder *coder);
bool wait_dongles_to_cold(t_dongle *dongle, long time_to_get_cold);

bool init_dongles(t_representer *representer);
void make_dongles_unavailable(t_dongle *dongle);
void dongles_destroyer(t_dongle **dongles, int counter);


// ============= Initializer ====================>
bool init_representer_mutexs_conds(t_representer *representer);
bool initialize_representer_struct(t_representer *representer, int ac, char **av);

// ============= Monitor ====================>
bool monitor_creator(t_representer *representer);
void monitor_joiner(pthread_t *monitor);
void *monitor_home(void *args) ;
bool wait_for_coders_to_start(t_representer *representer);


// ============= Mutexs ====================>
bool init_mutex_cond(t_mutex_cond *mutex_cond);
void destroy_mutex_cond(pthread_mutex_t *mutex, pthread_cond_t *cond);
void destroy_mutex_coders(t_coder **coders, int n);
bool init_coders_mutexes_conds(t_coder **coders, int number_of_coders);


// ============= Parser ====================>
int ft_strcmp(char *s1, char *s2);
bool parser(int ac, char **args, t_representer *representer);

// ============= Queue ====================>
void shift_queue_up(t_queue *queue, int index);
void swap_coders(t_coder *parent_coder, t_coder *child_coder);
void shift_queue_down(t_queue *queue, int i);
bool pop_coder_from_queue(t_representer *representer, int i);
void insert_coder_in_queue(t_coder *coder, t_queue *queue);


long timeval_to_ms(struct timeval time);
long get_time_ms();
int timeval_less(struct timeval a, struct timeval b);







#endif