#include "codexion.h"

void queue_up(t_queue *queue, int index)
{
    int parent;

    while (index > 0)
    {
        parent = (index - 1) / 2;
        if (queue)
        {
            if (queue->coders[parent]->deadline <= queue->coders[index]->deadline)
                break;
        }
        else  // fifo
        {
            if (queue->coders[parent]->access <= queue->coders[index]->access)
                break;
        }
        // swap_coders(&queue->coders[parent], &queue->coders[index]);
        index = parent;
    }
}


void insert_coder_in_queue(t_coder *coder, t_queue *queue)
{

    pthread_mutex_lock(&queue->mutex_queue);
    // don't insert if full or already in heap
    
    coder->access = get_time_ms();
    queue->coders[queue ->size] = coder;
    queue_up(queue, queue->size);
    queue->size++;
    pthread_mutex_unlock(&queue->mutex_queue);
}

