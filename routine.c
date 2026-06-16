#include"codexion.h"

void ft_compiling(t_coder *coder)
{
	t_queue *queue;
	queue  = coder->representer->queue;
	pthread_mutex_lock(&queue->mutex_queue);
	pthread_mutex_lock(&coder->mutex);
	
	printf("=================== here is the compiling test ========================\n");
	insert_coder_in_queue(coder, coder->representer);
    
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_unlock(&queue->mutex_queue);

	
	while(time_calculator(coder, coder->representer->coders[0]->last_compile) < coder->representer->config.dongle_cooldown)
		coder_must_wait(coder);
	
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&coder->mutex);
	
	drop_both_dongles(coder);

	*(coder->coder_state) = DEBUGING;
}
void coder_must_wait(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);

	while (*(coder->coder_state) == WAITING) 
		pthread_cond_wait(&coder->cond, &coder->mutex);
	pthread_mutex_unlock(&coder->mutex);
	pthread_mutex_unlock(&coder->mutex);

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
	while (1) {
		pthread_mutex_lock(&coder->mutex);
		ft_compiling(coder);
		printf("============ here is the routine test  ============\n");
		
		// ft_debuging(coder);
		// ft_refactoring(coder);
		pthread_mutex_unlock(&coder->mutex);
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
		// printf("here is the coders creation test\n");
		i++;
	}
}

void coders_joiner(t_representer *representer)
{
	int i;
	i = 0;
	
	while (i < representer->config.number_of_coders)
	{
		pthread_join(representer->coders[i]->thread, NULL);
		i++;
	}
}
