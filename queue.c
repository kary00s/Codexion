#include "codexion.h"

void shift_queue_up(t_queue *queue, int index)
{
    int parent;

    pthread_mutex_lock(&queue->mutex_queue);
    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (&queue->coders[parent]->last_compile <= &queue->coders[index]->last_compile)
            break;
        swap_coders(queue->coders[parent], queue->coders[index]);
        index = parent;
    }
    pthread_mutex_unlock(&queue->mutex_queue);
}
void swap_coders(t_coder *parent_coder, t_coder *child_coder)
{
    t_coder *swp;
    swp = parent_coder;
    parent_coder = child_coder;
    child_coder = swp;
}
void shift_queue_down(t_queue *queue, int i)
{


}
bool pop_coder_from_queue(t_representer *representer, int i)
{
    t_coder *coder;
    t_queue *queue;
    coder = representer->queue->coders[i];
    queue = representer->queue;

    if (!queue || (i > queue->size) || (queue->size > queue->capacity))
        return false;    
    swap_coders(coder, queue->coders[queue->size - 1]);
    queue->size--;
    if (representer->config.scheduler == 0)
        shift_queue_down(queue, i);
    // else
        // edf must be here akhona
    
    return true;    
}

void insert_coder_in_queue(t_coder *coder, t_queue *queue)
{
    pthread_mutex_lock(&coder->queue->mutex_queue);
    queue->coders[queue->size] = coder;
    queue->size++;
    pthread_mutex_unlock(&coder->queue->mutex_queue);

    // pthread_mutex_lock(&coder->mutex_cond.mutex);
    // printf("coder %d is inserted to queue\n", coder->coder_id);
    // pthread_mutex_u  nlock(&coder->mutex_cond.mutex);
}