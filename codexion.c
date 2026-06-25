#include"codexion.h"

int main(int ac, char *av[])
{
	t_representer representer;
	
	if (initialize_representer_struct(&representer, ac, av) == false)
		free_representer_struct(&representer);
	linker_coders_with_dongles(&representer);
	
	coders_creator(&representer);
	monitor_creator(&representer);
	manager_creator(&representer);

	coders_joiner(&representer);
	manager_joiner(representer.manager);
	monitor_joiner(representer.monitor);
	free_representer_struct(&representer);
}
