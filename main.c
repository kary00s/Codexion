#include"codexion.h"

long get_time_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000) ;
}
long theard_time()
{

}

void *coder_routine(void *arg) {
    t_coder *coder = (t_coder *)arg;
    printf("started => %ld %d \n", get_time_ms(), coder->coder_id);
    return NULL;
}
void creator(t_coder *coder)
{
	pthread_create(&(coder->thread), NULL, coder_routine, coder);
	pthread_join(coder->thread, NULL);
}

int main(int ac, char *av[])
{
	t_scene *scene;
	t_coder *coder;

	if (ac == 1 )
	{
		// 1- parsing the args 
		// parser(scene, av);	
		// 2- creating the scene
		creator(coder);
		// creator()
	}
	else
	{
		return 42;
	}
	return 0;
}