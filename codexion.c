#include"codexion.h"

int main(int ac, char *av[])
{
	t_representer *representer;
	representer = (t_representer *)malloc(sizeof(t_representer));

	initialize_representer_struct(representer, ac, av);
	linker_coders_with_dongles(representer);
	

	coders_creator(representer);
	// monitor_creator(representer);
	// manager_creator(representer);
	
	
	coders_joiner(representer);
	// manager_joiner(representer->manager);
	// monitor_joiner(representer->monitor);
	free_representer_struct(representer);
}
