int main(int ac, char *av[])
{
	t_scene *scene = malloc(sizeof(t_scene));
	// 1- parsing the args 
	parser(scene, ac, av);	
	scene->coder  = malloc(sizeof(t_coder)  * scene->number_of_coders);
	scene->dongle = malloc(sizeof(t_dongle) * scene->number_of_coders);

	// 2- creating the scene
	dongles_creator(scene);
	init_coders(scene);
	coders_creator(scene);
	wait_all_coders(scene);

	free (scene);
}
