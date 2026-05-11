#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>

pthread_cond_t = 
void *routine(void *args)
{
    while (1)
    {        
        printf("created succeffully\n");
        pthread_cond_wait()
    }
}
int main(void)
{
    pthread_t *thread;
    long id = pthread_self();
    pthread_create(&thread, NULL, routine, NULL);
    printf("%ld\n", id);
}