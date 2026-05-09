#include <stdio.h>
#include <stdlib.h>

typedef struct s_node
{
    int             value;
    struct s_node   *next;
}   t_node;

typedef struct s_queue
{
    t_node *front;
    t_node *rear;
}   t_queue;

void enqueue(t_queue *queue, int value)
{
    t_node *new_node;

    new_node = malloc(sizeof(t_node));
    if (!new_node)
        return;

    new_node->value = value;
    new_node->next = NULL;

    // if queue is empty
    if (queue->rear == NULL)
    {
        queue->front = new_node;
        queue->rear = new_node;
        return;
    }

    // append at the end
    queue->rear->next = new_node;
    queue->rear = new_node;
}

void print_queue(t_queue *queue)
{
    t_node *tmp = queue->front;

    while (tmp)
    {
        printf("%d ", tmp->value);
        tmp = tmp->next;
    }
    printf("\n");
}

int main(void)
{
    t_queue queue;

    queue.front = NULL;
    queue.rear = NULL;

    enqueue(&queue, 10);
    enqueue(&queue, 20);
    enqueue(&queue, 30);

    print_queue(&queue);
    return 0;
}