#include "codexion.h"


static int enqueue(t_queue *queue, int coder_id)
{
    t_queue_node *node = malloc(sizeof(t_queue_node));
    if (!node)
        return -1;
    node->id_node = coder_id;
    node->next = NULL;

    if (queue->next == NULL)
    {
        queue->prev = node; // front
        queue->next = node; // rear
    }

    return 0;
}

t_queue *queue_filler(t_representer *representer)
{
    t_queue *queue = malloc(sizeof(t_queue));
    if (!queue)
        return NULL;
    queue->prev = NULL;
    queue->next = NULL;

    for (int i = 0; i < representer->config.number_of_coders; i++)
    {
        enqueue(queue, representer->coders[i]->coder_id);
    }
    return queue;
}