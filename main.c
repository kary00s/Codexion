#include"codexion.h"

int main(int ac, char *av[])
{
	t_representer *representer;
	representer = (t_representer *)malloc(sizeof(t_representer));
	initialize_representer_struct(representer, ac, av);
	printf("=>%d\n",representer->config.number_of_coders);
	printf("=>%d\n",representer->config.time_to_debug);
	printf("=>%d\n",representer->config.time_to_burnout);
	printf("=>%d\n",representer->config.time_to_compile);
	printf("=>%d\n",representer->config.time_to_refactor);
	printf("=>%d\n",representer->config.number_of_compiles_required);
	printf("=>%d\n",representer->config.dongle_cooldown);

	free_representer_struct(representer);
}
