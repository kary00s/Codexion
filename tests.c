#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include <sys/time.h>
typedef struct s_node
{
	int node_id;
	time_t last_compile;
} t_node;
 
typedef struct s_heap
{
	t_node **nodes;
	int size;
	int capacity;
	pthread_mutex_t mutex_heap;
} t_heap;

int main()
{
	t_heap	*heap;
	heap = malloc(sizeof(t_heap));
	heap->size = 0;
	heap->capacity = 3;
	heap->nodes = malloc(sizeof(t_node *) * heap->capacity);
	pthread_mutex_init(&heap->mutex_heap, NULL);

	t_node *node1;
	node1->last_compile = 10;
	node1->node_id = 1;

	t_node *node2;
	node2->last_compile = 20;
	node2->node_id = 2;
	
	t_node *node3;
	node3->last_compile = 30;
	node3->node_id = 3;
	
	insert_to_heap(node1, heap);
}