#include"codexion.h"

int main(int ac, char *av[])
{
	t_scene *scene;
	if (ac == 1 || ac == 2)
	{
		// 1- parsing the args 
		parser(scene, av);
		printf("%d\n", scene->number_of_coders);

	
		// 2- creating the scene
		// creator()
	}
	else
	{
		return 42;
	}
}