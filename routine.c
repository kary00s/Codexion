#include"codexion.h"

void ft_compiling(t_coder *coder,  t_queue *queue)
{
	
	// insert_coder_in_queue(coder, queue);
	
	pthread_mutex_lock(&coder->mutex);
	while (*(coder->coder_state) == WAITING) 
		pthread_cond_wait(&coder->cond, &coder->mutex);
	printf("hereeeeeeeeeeeee => %d\n", coder->coder_id);

	pthread_mutex_unlock(&coder->mutex);
	
	pthread_mutex_lock(&coder->mutex);

	while(time_calculator(coder, coder->last_compile) < coder->config->dongle_cooldown)
		usleep(100);
	printf(" %d coder is compiling\n", coder->coder_id);

	coder->last_compile = get_time_ms();

	drop_both_dongles(coder);

	*(coder->coder_state) = DEBUGING;
}

void ft_debuging(t_coder *coder) {
	// print coder debuging 
	if (*(coder->coder_state) == DEBUGING)
	{		
		printf(" %d coder is debuging\n", coder->coder_id);
		
		// sleep the time of debuging
		usleep(coder->config->time_to_debug);
		
		// change coder state to refactoring
		*(coder->coder_state) = REFACTORING;
	}
}

void ft_refactoring(t_coder *coder) {
	// log coder refactoring
	if(*(coder->coder_state) == REFACTORING)
	{
		printf(" %d coder is debuging\n", coder->coder_id);

		// sleep time of refactoring
		usleep(coder->config->time_to_refactor);

		// change coder state to waiting
		*(coder->coder_state) = WAITING;
	}
}
void *routine_all_the_coders(void *rpster)
{
	t_representer *representer ;
	representer = (t_representer *)rpster;
	int i = 0;
	
	while(1) {
		if(representer->coders_are_ready == true)
		{
			ft_compiling(representer->coders[i], representer->queue);
			printf("the coder %d\n", representer->coders[i]->coder_id);
			ft_debuging(representer->coders[i]);
			ft_refactoring(representer->coders[i]);
		}
		if (i == representer->queue->size)
			i = 0;
		i++;
	}
	return NULL;
}

void threads_creator(t_representer *representer)
{
	int i = 0;
	
	while (i < representer->config.number_of_coders)
	{
		pthread_create(&representer->coders[i]->thread, NULL, routine_all_the_coders, (void *)representer);
		// printf("karim taeeeeeeeee\n");
		i++;
	}
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
	pthread_join(representer->monitor->monitor, NULL);
}
