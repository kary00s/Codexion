#ifndef CODEXION_H
#define CODEXION_H

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
typedef struct s_manager t_manager;

typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
typedef struct s_representer  t_representer;
typedef struct s_config t_config;
typedef enum e_coder_state t_coder_state;


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
    int             dongle_id;
    bool is_available;
   	pthread_mutex_t dongle_mutex;
	pthread_cond_t dongle_cond;
} t_dongle;

// CODERS struct :
typedef struct s_coder
{
	int coder_id;
	int coders_counter;
	pthread_mutex_t burnout_mutex;
	pthread_t thread;
	time_t				access;	
	time_t				last_compile;
  t_queue *queue;
  t_config *config;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	t_coder_state *coder_state;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	bool is_burnouted;
  pthread_mutex_t *print_mutex;
}	t_coder;

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
    DEBUGING,
    COMPILING,
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

typedef struct s_monitor
{
    pthread_t monitor;
    pthread_cond_t cond_monitor;    
    pthread_mutex_t mutex_monitor; 

} t_monitor ;


typedef struct s_mutex_cond {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} t_mutex_cond;

typedef struct s_representer
{
    int coders_counter;
    t_config config;
    t_dongle **dongles;
    t_coder **coders;
    bool coders_are_ready;
    bool is_burnouted;
    t_queue *queue;
    t_mutex_cond m_c;
    t_monitor *monitor;
    t_manager *manager;
    pthread_mutex_t print_mutex;
    int ready_coders_counter;
    t_mutex_cond ready_coders_counter_m_c;
} t_representer;

typedef struct s_manager
{
    pthread_t manager;
    pthread_cond_t cond_manager;    
    pthread_mutex_t mutex_manager; 
} t_manager;



void coder_must_wait(t_coder *coder);

// codexion file :
int main(int ac, char *av[]);
void dongles_destroyer(t_dongle **dongles, int counter);
t_config parser(int ac, char **args);
void exit_all(char *message);
void coders_creator(t_representer *representer);


// dongles file :
void init_dongles(t_representer *representer);
bool are_dongles_available(t_coder *coder);

// coders file :
void   init_coders(t_representer *representer);

void coders_joiner(t_representer *representer);
void *routine_all_the_coders(void *arg);


// monitor file :
t_monitor *monitor_initializer(void);
t_representer *linker_coders_with_dongles(t_representer *representer);
void *monitor_home(void *args);
void monitor_joiner(t_monitor *monitor);
void monitor_creator(t_representer *representer);

// manager_file :
void *manager_home(void *args);
void manager_joiner(t_manager *manager);
void manager_creator(t_representer *representer);
t_coder *peek_a_coder(t_representer *representer);
t_manager *manager_initializer(void);


// cleaner file :
void free_dongles(t_representer *representer);
void free_representer_struct(t_representer *representer);
void free_coders(t_representer *representer);


// timer file:
long time_calculator(t_coder *coder, long start_time);
long get_time_ms();

// queue file :
void insert_coder_in_queue(t_coder *coder, t_queue *queue);
// void insert_all_coders_in_queue(t_representer *representer, t_queue *queue);

// droper file :
void drop_both_dongles(t_coder *coder);

// holder file : 
void hold_both_dongles(t_coder *coder);

void swap_coders(t_coder *parent_coder, t_coder *child_coder);

// initializer file:
t_queue *initializer_queue(t_representer *representer);
t_representer *initialize_representer_struct(t_representer *representer ,int ac, char **av);


// mutex_cond_utils:
bool init_mutex_cond(t_mutex_cond *mutex_cond);
void destroy_mutex_cond(t_mutex_cond *mutex_cond);

#endif
