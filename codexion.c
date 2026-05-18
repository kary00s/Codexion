#include"codexion.h"

void threads_creator(t_representer *representer)
{
	int i = 0;

	while (i < representer->config.number_of_coders)
	{
		pthread_create(&representer->coders[i]->thread, NULL, routine_coders, (void *)representer);
		i++;
	}
}


int main(int ac, char *av[])
{
	t_representer *representer;
	representer = (t_representer *)malloc(sizeof(t_representer));
	initialize_representer_struct(representer, ac, av);
	
	threads_creator(representer);
	threads_joiner(representer);
	
	
	linker_coders_with_dongles(representer);
	representer->queue = queue_filler(representer);

	// compile_coders(representer);
	free_representer_struct(representer);
}
