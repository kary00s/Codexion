#include"codexion.h"

long get_time_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000) ;
}
long time_to_compile(t_coder *coder, long start_time)
{
	printf("the %ld is compiling\n", coder->coder_id);
	usleep(1000000);	
	long now = get_time_ms();
	return now - start_time;
}