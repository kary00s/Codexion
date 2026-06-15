#include"codexion.h"

void ft_compiling(t_coder *coder)
{
	pthread_mutex_lock(&coder->representer->queue->mutex_queue);
	insert_coder_in_queue(coder, coder->representer->queue);
	
	while(time_calculator(coder, coder->last_compile) < coder->config->dongle_cooldown)
		usleep(100);
	pthread_mutex_unlock(&coder->representer->queue->mutex_queue);

	coder->last_compile = get_time_ms();

	drop_both_dongles(coder);

	*(coder->coder_state) = DEBUGING;
	printf("compiling is done\n");
}
bool coder_must_wait(t_coder *coder)
{
	while (*(coder->coder_state) == WAITING) 
		pthread_cond_wait(&coder->cond, &coder->mutex);
	pthread_mutex_unlock(&coder->mutex);
	return false;
}


void ft_debuging(t_coder *coder) {
	if (*(coder->coder_state) == DEBUGING)
	{		
		printf(" %d coder is debuging\n", coder->coder_id);
		
		usleep(coder->config->time_to_debug);
		
		*(coder->coder_state) = REFACTORING;
	}
}

void ft_refactoring(t_coder *coder) {
	if(*(coder->coder_state) == REFACTORING)
	{
		printf(" %d coder is debuging\n", coder->coder_id);
		usleep(coder->config->time_to_refactor);
		*(coder->coder_state) = WAITING;
	}
}
void *routine_all_the_coders(void *arg)
{
	t_coder *coder ;
	coder = (t_coder *)arg;
	while(1) {
		
		// ft_compiling(coder);
		// ft_debuging(coder);
		// ft_refactoring(coder);
	}
	return NULL;
}

void coders_creator(t_representer *representer)
{
	int i = 0;
	
	// everything works untill here
	while (i < representer->config.number_of_coders)
	{
		pthread_create(&representer->coders[i]->thread, NULL, routine_all_the_coders, (void *)representer->coders[i]);
		printf("coder %d\n", representer->coders[i]->coder_id);
		i++;
	}
}

void coders_joiner(t_representer *representer)
{
	int i;
	i = 0;
	
	while (i < representer->config.number_of_coders)
	{
		printf("coder raqm %d\n", representer->config.number_of_coders);
		pthread_join(representer->coders[i]->thread, NULL);
		i++;
	}
}
