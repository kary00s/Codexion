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
#include <errno.h>


typedef struct s_controller t_controller;

typedef struct timespec t_timespec;
typedef struct timeval t_timeval;
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
    struct timeval   last_reste;
    bool          is_available;
    bool          is_cold;
    // t_timeval cooldown_timer;
    t_timespec time_reste;
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
    START,
    WAIT,
    COMPILING,
    DEBUGING,
    REFACTORING,
    EXIT,
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
	struct timeval  last_compile;
	pthread_mutex_t burnout_mutex;
	pthread_t thread;
  t_queue *queue;
  t_config *config;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	t_coder_state coder_state;
  t_mutex_cond mutex_cond;
	bool *is_burnout;
  pthread_mutex_t *is_burnout_mutex;
  pthread_mutex_t *print_mutex;
  int *ready_coders_counter;
  t_mutex_cond *ready_coders_counter_m_c;
  struct timeval *begining_time;
  t_mutex_cond numbers_compilation_m_c;
  int numbers_compilation;
  int *finished_coders;
  pthread_mutex_t *finished_coders_mutex;
}	t_coder;


typedef struct s_representer
{
  t_config config;
  t_dongle **dongles;
  t_coder **coders;
  bool coders_are_ready;
  t_queue *queue;
  pthread_mutex_t print_mutex;
  t_mutex_cond ready_coders_counter_m_c;
  pthread_mutex_t is_burnout_mutex;
  bool is_burnout;
  int required_numbers_compilation_is_completed;
  int ready_coders_counter;
  struct timeval begining_time;
  pthread_t monitor;
  pthread_t controller;
  int finshed_coders;
  pthread_mutex_t  finished_coders_mutex;
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
bool   is_coder_burnouted(t_coder *coder);
void coders_joiner(t_representer *representer);

void linker_coders_with_dongles(t_coder **coders, t_dongle **dongles, int number_of_coders);
bool	coder_waiting_dongles(t_coder *coder);
bool are_one_of_coders_burnout(t_representer *representer);

// ============= Controller ====================>
//=> controller.c
bool controller_creator(t_representer *representer);
void *controller_home(void *args) ;
void controller_joiner(pthread_t *controller);
t_coder *catch_coder(t_representer *representer);
bool is_representation_works_well(pthread_mutex_t *is_burnout_mutex, bool *is_burnout);

// ============= Routine ====================>
void *routine_all_the_coders(void *arg);
bool wait_for_simulation_to_start(t_coder *coder);
bool is_represontation_done(t_representer * representer);
bool wait(pthread_mutex_t *mutex, pthread_cond_t *cond, unsigned long time);
void add_coder_to_finished_coders(t_coder *coder) ;

bool compiling(t_coder *coder, t_queue *queue);
bool debuging(t_coder *coder);
bool refactoring(t_coder *coder);
void print_action(t_coder *coder);

// ============= Dongles ====================>
bool drop_both_dongles(t_coder *coder);
bool are_dongles_available(t_coder *coder);
bool is_the_dongle_cold(t_dongle *dongle, unsigned long time_cooldown);
long get_the_hotest_dongle(t_dongle *left_dongle, t_dongle *right_dongle);
bool check_dongles_coldness(t_coder *coder);


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
void exit_representation(t_representer *representer);


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
bool pop_coder_from_queue(t_representer *representer, int i);
void insert_coder_in_queue(t_coder *coder, t_queue *queue);
bool initializer_queue(t_representer *representer) ;
bool init_queue_mutexs_conds(t_representer *representer) ;


// ============= timer ====================>
unsigned long timeval_to_ms(struct timeval time);
unsigned long get_time_ms();
int timeval_less(struct timeval a, struct timeval b);
void	ms_to_timespec(t_timespec *timespec, unsigned long time_ms);
long time_elapsed_until_now(struct timeval elapsed_time);
void register_time(struct timeval *time, pthread_mutex_t *mutex);


#endif
