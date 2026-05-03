#ifndef CODEXION
#define CODEXION

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
typedef struct s_scene t_scene;
typedef struct s_dongle
{
    pthread_mutex_t mutex;
    int             dongle_id;
} t_dongle;
typedef struct s_coder
{
    int             coder_id;
    pthread_t       thread;
    t_dongle       *left;
    t_dongle       *right;
    t_scene        *scene;

} t_coder;


typedef struct s_scene
{
    int number_of_coders;
    int time_to_debug;
    int time_to_burnout;
    int  time_to_compile;
    int  time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;

    t_dongle *dongle;
    t_coder *coder;

} t_scene;

//  number_of_compiles_required // dongle_cooldown 
// scheduler
void coders_creator(t_scene *scene);
void dongles_creator(t_scene *scene);
void wait_all_coders(t_scene *scene);

long time_to_compile(t_coder *coder, long start_time);
long get_time_ms();
void exit_all();
void coders_creator(t_scene *scene);
void parser(t_scene *scene,int ac, char **args);

#endif