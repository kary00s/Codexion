#include"codexion.h"

void threads_creator(t_representer *representer)
{
	int i = 0;

	pthread_t *manager;
	if (pthread_create(&manager, NULL, &monitor, representer))
	while (i < representer->config.number_of_coders)
	{
		// if (representer->coders[i]->coder_id % 2 == 0)
			// coder_hold_both_dongles(representer->coders[i]);
		pthread_create(&representer->coders[i]->thread, NULL, routine_all_the_coders, (void *)representer);
		i++;
	}
	
}


int main(int ac, char *av[])
{
	t_representer *representer;
	representer = (t_representer *)malloc(sizeof(t_representer));
	
	initialize_representer_struct(representer, ac, av);
	linker_coders_with_dongles(representer);
	

	threads_creator(representer);
	threads_joiner(representer);
	
	free_representer_struct(representer);
}
