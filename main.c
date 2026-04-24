#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


pthread_mutex_t mutex_var;
pthread_cond_t cond_var;

int	arraylen(int *arr)
{
	if (!arr)
		return 0;
	int i = 0;
	while (arr[i])
		i += 1;
	return (i);
}

void	*producer(void *arg)
{
	int	*arr = (int *)arg;
	
	pthread_mutex_lock(&mutex_var);
	printf("producer: im filling buffer\n");
	pthread_mutex_unlock(&mutex_var);

	for (int i = 0; i < 10; i++)
	{
		pthread_mutex_lock(&mutex_var);
		arr[i] = i + 1;
		printf("arr[%d]: %d\n", i, arr[i]);
		pthread_mutex_unlock(&mutex_var);
	}
	pthread_mutex_lock(&mutex_var);
	pthread_cond_signal(&cond_var);
	pthread_mutex_unlock(&mutex_var);
	return (NULL);
}

void	*consumer(void *arg)
{
	int	*arr = (int *)arg;

	pthread_mutex_lock(&mutex_var);

	while (arraylen(arr) < 10)
	{
		pthread_mutex_unlock(&mutex_var);
		usleep(200);
		pthread_mutex_lock(&mutex_var);
	}
	pthread_mutex_unlock(&mutex_var);
	
	usleep(500);
	
	pthread_mutex_lock(&mutex_var);
	printf("Consumer: im Consume buffer Now!\n");
	for (int i = 0; i < 10; i++)
		printf("arr[%d] ==> %d\n", i, arr[i]);
	for (int i = 0; i < 10; i++)
		arr[i] = 0;
	pthread_mutex_unlock(&mutex_var);
	return (NULL);
}


int	main(void)
{
	pthread_t	t1, t2;

	int	*arr = malloc(sizeof(int) * 10);
	
	if (!arr)
	{
		printf("Error malloc\n");
		return (42);
	}

	pthread_mutex_init(&mutex_var, NULL);
	pthread_cond_init(&cond_var, NULL);
	
	if (pthread_create(&t1, NULL, producer, (void *)arr))
	{
		printf("Error: producer failed\n");
		return (42);
	}

	if (pthread_create(&t2, NULL, consumer, (void *)arr))
	{
		printf("Error: consumer failed\n");
		return (42);
	}
	
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

}