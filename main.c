#include"codexion.h"

static void *coder_routine(void *arg) {
	long start_time = get_time_ms();
	t_coder *coder = (t_coder *)arg;
    printf("started => %ld  \n\n", time_to_compile(coder, start_time));
    return NULL;
}
void coders_creator(t_scene *scene)
{
	int i = 0;
	while (i < scene->number_of_coders)
	{
		scene->coder->coder_id = i;
		pthread_create(&(scene->coder->thread), NULL, coder_routine, scene->coder);
		pthread_join(scene->coder->thread, NULL);
		i++;
	}
	
}

int main(int ac, char *av[])
{
	t_scene *scene = malloc(sizeof(t_scene));
	// 1- parsing the args 
	parser(scene, ac, av);	
	// 2- creating the scene
	scene->coder = malloc(sizeof(scene->coder->thread) + scene->number_of_coders);
	coders_creator(scene);
	
	
	// printf("%d\n",scene->number_of_coders);
    // printf("%ld\n",scene->time_to_burnout);
    // printf("%ld\n",scene->time_to_compile);
    // printf("%ld\n",scene->time_to_debug);
    // printf("%ld\n",scene->time_to_refactor);
    // printf("%ld\n",scene->number_of_compiles_required);
    // printf("%ld\n",scene->dongle->dongle_cooldown);
}
