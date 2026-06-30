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



bool init_coders(t_representer *representer) ;
t_coder **coders_allocater(int number_of_coders);
static void initialize_coders_struct(t_representer *representer);
