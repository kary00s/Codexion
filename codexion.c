#include"codexion.h"

void threads_creator(t_representer *representer)
{
	int i = 0;
	while (i < representer->config.number_of_coders)
		pthread_create(&representer->coders[i++]->thread, NULL, routine_all_the_coders, (void *)representer);
}	

int main(int ac, char *av[])
{
	t_representer *representer;
	representer = (t_representer *)malloc(sizeof(t_representer));

	initialize_representer_struct(representer, ac, av);
	linker_coders_with_dongles(representer);
	threads_creator(representer);

	monitor_creator(representer);
	manager_creator(representer);

	threads_joiner(representer);
	monitor_joiner(representer->monitor);
	manager_joiner(representer->manager);

	free_representer_struct(representer);
}
