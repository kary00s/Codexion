#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<time.h>
#include<sys/time.h>
#include<unistd.h>
pthread_cond_t cond;
pthread_mutex_t mutex ;
// void *routine(void *args)
// {

//     while (1)
//     {        
//         pthread_mutex_lock(&mutex);
//         printf("created succeffully\n");
//         pthread_mutex_unlock(&mutex);
//         printf("is running\n");
//         pthread_cond_wait(&cond, &mutex);
//         sleep(1);   
//     }
//     return NULL;
// }
// int main(void)
// {
//     pthread_t thread;
//     pthread_t thread1;

//     pthread_create(&thread, NULL, routine, NULL);
//     pthread_create(&thread1, NULL, routine, NULL);
//     pthread_join(thread, NULL);
//     pthread_join(thread1, NULL);
    
//     return 0;
// }
long get_time_ms()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000) + (time.tv_usec / 1000) ;
}
int main()
{
    printf("%ld\n", get_time_ms());
}