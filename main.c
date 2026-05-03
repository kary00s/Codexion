#include"codexion.h"

static void *coder_routine(void *arg) {
	long start_time = get_time_ms();
	t_coder *coder = (t_coder *)arg;
    printf("started => %ld  \n\n", time_to_compile(coder, start_time));
    return NULL;
}


void wait_all_coders(t_scene *scene)
{
	int i = 0;

	while (i < scene->number_of_coders)
	{
		pthread_join(scene->coder[i].thread, NULL);
		i++;
	}
}
void dongles_creator(t_scene *scene)
{
	int i = 0;

	while (i < scene->number_of_coders)
	{
		scene->dongle[i].dongle_id = i;
		pthread_mutex_init(&scene->dongle[i].mutex, NULL);
		i++;
	}
}
void coders_creator(t_scene *scene)
{
	int i;

	i = 0;
	while (i < scene->number_of_coders)
	{
		scene->coder[i].coder_id = i;
		scene->coder[i].scene = scene;

		pthread_create(&scene->coder[i].thread,
			NULL,
			coder_routine,
			&scene->coder[i]);
		i++;
	}
}
void init_coders(t_scene *scene)
{
	int i = 0;

	while (i < scene->number_of_coders)
	{
		scene->coder[i].left = &scene->dongle[i];
		scene->coder[i].right = &scene->dongle[(i + 1)
			% scene->number_of_coders];
		i++;
	}
}

int main(int ac, char *av[])
{
	t_scene *scene = malloc(sizeof(t_scene));
	// 1- parsing the args 
	parser(scene, ac, av);	
	scene->coder  = malloc(sizeof(t_coder)  * scene->number_of_coders);
	scene->dongle = malloc(sizeof(t_dongle) * scene->number_of_coders);

	// 2- creating the scene
	dongles_creator(scene);
	init_coders(scene);
	coders_creator(scene);
	wait_all_coders(scene);
	free (scene);
}
