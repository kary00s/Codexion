#ifndef CODEXION_H
#define CODEXION_H

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct s_dongle t_dongle;
typedef struct s_coder t_coder;
typedef struct s_representer  t_representer;
typedef struct s_config t_config;
typedef enum e_coder_state t_coder_state;


typedef struct s_dongle
{
    int             dongle_id;
    bool is_available;
   	pthread_mutex_t dongle_mutex;
	pthread_cond_t dongle_cond;
} t_dongle;

typedef struct s_coder
{
	int coder_id;
	int coders_counter;
	pthread_mutex_t *burnout_mutex;
	pthread_t thread;
	t_dongle *left_dongle;
	t_dongle *right_dongle;
	t_coder_state *coder_state;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	bool is_burnouted;
}	t_coder;


typedef enum enum_scheduer
{
    FIFO,
    EDF
}   t_scheduler;

typedef enum e_coder_state
{
    STARTING,
    WAITING,
    DEBUGING,
    COMPILING,
    REFACTORING,
    FINISHING
}   t_coder_state;

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

typedef struct s_representer
{
    int coders_counter;
    t_config config;
    t_dongle **dongles;
    t_coder **coders;
	pthread_mutex_t burnout_mutex;
    bool is_burnouted;
    pthread_mutex_t mutex;
	pthread_cond_t cond;

} t_representer;

int main(int ac, char *av[]);
void dongles_destroyer(t_dongle **dongles, int counter);
t_config parser(int ac, char **args);
void exit_all(char *message);
void init_dongles(t_representer *representer);
void   init_coders(t_representer *representer);

// monitor file :
void linker_coders_with_dongles(t_representer *representer);
void initialize_representer_struct(t_representer *representer ,int ac, char **av);

// cleaner file :
void free_dongles(t_representer *representer);
void free_representer_struct(t_representer *representer);
void free_coders(t_representer *representer);

// timer file:
long time_to_compile(t_coder *coder, long start_time);
long get_time_ms();

#endif