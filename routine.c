#include"codexion.h"
void *routine_all_the_coders(void *rpster)
{
	t_representer *representer ;
	representer = (t_representer *)rpster;
	pthread_mutex_lock(&representer->mutex);
	representer->coders_counter++; 
	
	
	while (representer->coders_counter < representer->config.number_of_coders)
	{
		queue_filler(representer->coders[representer->coders_counter], representer->queue);
		pthread_cond_wait(&representer->cond, &representer->mutex);

	}
	if (representer->coders_counter == representer->config.number_of_coders)
	{
		representer->coders_are_ready = true;
		pthread_cond_broadcast(&representer->cond);
		pthread_mutex_unlock(&representer->mutex);
		printf("All coders are ready, starting work!\n");
	}
	return NULL;
}
void threads_joiner(t_representer *representer)
{
	int i;
	i = 0;
	
	while (i < representer->config.number_of_coders)
	{
		pthread_join(representer->coders[i]->thread, NULL);
		i++;
	}
}






// void ft_compile()
// {
// 	printf("is compiling\n");
// 	usleep(1000000);
// }
// static void ft_debug()
// {
// 	printf("is debugging\n");
// 	usleep(1000000);
// }
// static void ft_refactor()
// {
// 	printf("is refactoring\n");
// 	usleep(1000000);
// }