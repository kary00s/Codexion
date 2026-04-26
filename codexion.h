#ifndef CODEXION
#define CODEXION

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

typedef struct dgl 
{
    int             dongle_id;
    pthread_mutex_t dongle; 
    long dongle_cooldown;
}             t_dongle;



typedef struct coder
{
    int             coder_id;
    long            last_dongle_time;
    
    long time_to_debug;
    long time_to_burnout;
    long  time_to_compile ;
    long  time_to_refactor;

    pthread_t       coder;
    t_dongle          *right_coder;
    t_dongle          *left_coder;
}  t_coder;

typedef struct s_scene
{
    int number_of_coders;
    long die_time;
    long act_time;
    long wait_time;
    t_dongle *dongle;
    t_coder *coder;


} t_scene;

  
//  number_of_compiles_required // dongle_cooldown 
// scheduler 

void parser(t_scene *scene, char **args);


#endif