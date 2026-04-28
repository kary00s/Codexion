#ifndef CODEXION
#define CODEXION

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct dgl 
{
    long             dongle_id;
    pthread_mutex_t dongle; 
    long dongle_cooldown;
}             t_dongle;



typedef struct coder
{
    long             coder_id;
    long            last_compile_start;
    
    long last_time_to_debug;
    long last_time_to_burnout;
    long  last_time_to_compile ;
    long  last_time_to_refactor;

    pthread_t       thread;
    t_dongle          *right_dongle;
    t_dongle          *left_dongle;
}  t_coder;

typedef struct s_scene 
{
    int number_of_coders;
    
    long time_to_debug;
    long time_to_burnout;
    long  time_to_compile ;
    long  time_to_refactor;
    long number_of_compiles_required;
    long dongle_cooldown;
    t_dongle *dongle;
    t_coder *coder;

} t_scene;

//  number_of_compiles_required // dongle_cooldown 
// scheduler

long time_to_compile(t_coder *coder, long start_time);
long get_time_ms();
void exit_all();
void coders_creator(t_scene *scene);
void parser(t_scene *scene,int ac, char **args);
#endif