#include"codexion.h"


void *routine(void *args)
{	t_representer *representer;
	representer = (t_representer *)args;
	pthread_mutex_lock(&representer->mutex);
	printf("coder created succefully\n");
	pthread_mutex_unlock(&representer->mutex);

	// pthread_mutex_lock(&representer->mutex);
	// pthread_cond_signal(&representer->cond);
	// pthread_mutex_unlock(&representer->mutex);
	return(NULL);
}
void 	threads_joiner(t_representer *representer)
{
	int i;
	i = 0;

	while (i < representer->config.number_of_coders)
	{
		pthread_join(representer->coders[i]->thread, NULL);
		i++;
	}
	
}
void threads_creator(t_representer *representer)
{
	int i = 0;

	while (i < representer->config.number_of_coders)
	{
		pthread_create(&representer->coders[i]->thread, NULL, routine, (void *)representer);
		i++;
	}
	threads_joiner(representer);
}


int main(int ac, char *av[])
{
	t_representer *representer;
	representer = (t_representer *)malloc(sizeof(t_representer));
	initialize_representer_struct(representer, ac, av);
	printf("=>%d\n",representer->config.number_of_coders);
	printf("=>%d\n",representer->config.time_to_debug);
	printf("=>%d\n",representer->config.time_to_burnout);
	printf("=>%d\n",representer->config.time_to_compile);
	printf("=>%d\n",representer->config.time_to_refactor);
	printf("=>%d\n",representer->config.number_of_compiles_required);
	printf("=>%d\n",representer->config.dongle_cooldown);

	threads_creator(representer);
	linker_coders_with_dongles(representer);


	free_representer_struct(representer);
}
