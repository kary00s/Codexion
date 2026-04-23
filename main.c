#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
void *routine(void *arg)
{
    int id = pthread_self();
    printf("%d\n", id);
    return NULL;
}
int main(void)
{
    pthread_mutex_t lock;

    int i = 0;

    pthread_t id_thread;

    printf("%d\n", i );
    printf("=========\n");
    int idt = pthread_self();
    printf("%d\n", idt);
    while (i < 5)
    {
        pthread_mutex_lock(&lock);
        pthread_create(&id_thread, NULL, routine, &i);
        pthread_join(id_thread, NULL);
        i++;
        printf("%p\n",&lock);
        pthread_mutex_unlock(&lock);

    }
    printf("=========\n");
}