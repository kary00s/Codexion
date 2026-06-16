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

void insert_coder_in_queue(t_coder *coder, t_representer *representer)
{
    coder->access = get_time_ms();
	printf("==> inserting coder to queue test \n");
    representer->coders[representer->queue->size] = coder;
    shift_queue_up(representer->queue, representer->queue->size);
    
    representer->queue->size++;

}



// void insert_all_coders_in_queue(t_representer *representer, t_queue *queue)
// {
// 
    // int i;
    // i = 0;
    // printf("size = %d\n", queue->size);
    // while (i < representer->config.number_of_coders)
    // {
        // pthread_mutex_lock(&queue->mutex_queue);
        // representer->coders[i]->access = get_time_ms();
        // queue->coders[queue->size] = representer->coders[i];
        // queue->size++;
        // pthread_mutex_unlock(&queue->mutex_queue);
        // i++;
    // }
    // representer->coders_are_ready = true;
// }
