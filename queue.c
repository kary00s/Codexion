#include "codexion.h"

void shift_queue_up(t_queue *queue, int index)
{
    int parent;

    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (queue->coders[parent]->access <= queue->coders[index]->access)
            break;
        swap_coders(queue->coders[parent], queue->coders[index]);
        index = parent;
    }
}
void swap_coders(t_coder *parent_coder, t_coder *child_coder)
{
    t_coder *swp;
    swp = parent_coder;
    parent_coder = child_coder;
    child_coder = swp;
}

void insert_coder_in_queue(t_coder *coder, t_queue *queue)
{
    pthread_mutex_lock(&coder->queue->mutex_queue);
    queue->coders[queue->size] = coder;
    queue->size++;
    pthread_mutex_unlock(&coder->queue->mutex_queue);

    pthread_mutex_lock(&coder->mutex_cond.mutex); 
    // printf("coder %d is inserted to queue\n", coder->coder_id);
    pthread_mutex_unlock(&coder->mutex_cond.mutex);   

}