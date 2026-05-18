#include"codexion.h"
void routine_all_the_coders(t_representer *representer)
{ 
	pthread_mutex_lock(&representer->mutex);
	representer->coders_counter++; 
	if (representer->coders_counter < representer->config.number_of_coders)
	{ 
		while (representer->coders_counter < representer->config.number_of_coders)
			pthread_cond_wait(&representer->cond, &representer->mutex);
	}	
	else 
	{
		pthread_cond_broadcast(&representer->cond);
		pthread_mutex_unlock(&representer->mutex);
		printf("All coders are ready, starting work!\n");
	}
}
void ft_compile()
{
	printf("is compiling\n");
	usleep(1000000);
}
static void ft_debug()
{
	printf("is debugging\n");
	usleep(1000000);
}
static void ft_refactor()
{
	printf("is refactoring\n");
	usleep(1000000);
}
void *routine_coders(void *args)
{
    t_representer *representer = (t_representer *)args;
	routine_all_the_coders(representer);
	while (1)
	{
		ft_compile();
		ft_debug();
		ft_refactor();
	}
    return NULL;
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