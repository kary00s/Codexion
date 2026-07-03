#include "../codexion.h"
static void shift_queue_down_fifo(t_queue *queue, int i);
static void shift_queue_down_edf(t_queue *queue, int i);

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

static void shift_queue_down_edf(t_queue *queue, int i)
{
	int		right;
	int		left;
	int		smallest;

    while (1)
    {
        right = i * 2 + 2;
        left = i * 2 + 1;
        smallest = i;
        
        if (left < queue->size &&
            timeval_less(queue->coders[left]->last_compile,
                    queue->coders[smallest]->last_compile))
            smallest = left;
    
        if (right < queue->size &&
            timeval_less(queue->coders[right]->last_compile,
                    queue->coders[smallest]->last_compile))
            smallest = right;
        
        if (smallest == i)
            break;
        
        swap_coders(queue->coders[i], queue->coders[smallest]);
        i = smallest;
    }
}

bool pop_coder_from_queue(t_representer *representer, int i)
{
    t_coder *coder;
    t_queue *queue;
    coder = representer->queue->coders[i];
    queue = representer->queue;
  
    // swap_coders(coder, queue->coders[queue->size - 1]);
    if (representer->config.scheduler ==   FIFO)
        shift_queue_down_fifo(queue, i);
    else
        shift_queue_down_edf(queue, i);
    queue->size--;
    
    return true;    
}

static void shift_queue_down_fifo(t_queue *queue, int i)
{

    while (i < queue->size - 1)
    {
        queue->coders[i] = queue->coders[i+1];
        i++;
    }
}
void insert_coder_in_queue(t_coder *coder, t_queue *queue)
{
    pthread_mutex_lock(&coder->queue->mutex_queue);
    queue->coders[queue->size] = coder;
    queue->size++;
    pthread_mutex_unlock(&coder->queue->mutex_queue);
}
