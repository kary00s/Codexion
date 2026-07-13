#include "../codexion.h"

static void shift_queue_down_fifo(t_queue *queue, int i);
static void shift_queue_down_edf(t_queue *queue, int i);
static t_queue *queue_allocater(t_representer *representer);

void swap_coders(t_coder **parent_coder, t_coder **child_coder) 
{
  t_coder *swp;
  swp = *parent_coder;
  *parent_coder = *child_coder;
  *child_coder = swp;
}

static void shift_queue_down_edf(t_queue *queue, int i)
{
  int left;
  int right;
  int smallest;
  unsigned long smallest_time;
  unsigned long child_time;

  left = i * 2 + 1;
  right = i * 2 + 2;
  smallest = i;

  smallest_time = timeval_to_ms(queue->coders[i]->last_compile);
  if (left < queue->size)
  {
    child_time = timeval_to_ms(queue->coders[left]->last_compile);
    if (child_time < smallest_time)
    {
      smallest = left;
      smallest_time = child_time;
    }
  }
  if (right < queue->size)
  {
    child_time = timeval_to_ms(queue->coders[right]->last_compile);
    if (child_time < smallest_time)
    {
      smallest = right;
      smallest_time = child_time;
    }
  }
  if (smallest != i)
  {
    swap_coders(&queue->coders[i], &queue->coders[smallest]);
    shift_queue_down_edf(queue,i);
  }
}


bool pop_coder_from_queue(t_representer *representer, int i) 
{
  t_queue *queue;
  queue = representer->queue;
  if (representer->queue->coders[i] == NULL)
    return false;
    
  if (representer->config.scheduler == FIFO)
  {
    shift_queue_down_fifo(queue, i);
  }
  else if (representer->config.scheduler == EDF)
  {      
    swap_coders(&representer->queue->coders[0], &representer->queue->coders[i]);
    swap_coders(&queue->coders[0], &queue->coders[queue->size-1]);
    shift_queue_down_edf(queue, i);
  }
  queue->size--;
  return true;
}

static void shift_queue_down_fifo(t_queue *queue, int i) 
{

  while (i < queue->size - 1) {
    queue->coders[i] = queue->coders[i + 1];
    i++;
  }
}

void insert_coder_in_queue(t_coder *coder, t_queue *queue)
{
  pthread_mutex_lock(&queue->mutex_queue);
  queue->coders[queue->size] = coder;
  queue->size++;
  pthread_mutex_unlock(&queue->mutex_queue);
}

bool init_queue(t_representer *representer) {
  t_queue *queue;
  
  queue = queue_allocater(representer);
  if (queue == NULL)
    return false;
  queue->capacity = representer->config.number_of_coders;
  queue->size = 0;
  queue->coders = (t_coder **)malloc(sizeof(t_coder *) * representer->config.number_of_coders);
  if (queue->coders == NULL)
  {
    free(queue);
    return false;
  }
  representer->queue = queue;
  if (!init_queue_mutexs_conds(representer))
  {
    free(queue->coders);
    free(queue);
    return false;
  }
  return true;
}

static t_queue *queue_allocater(t_representer *representer)
{
  t_queue  *queue;
  queue = malloc(sizeof(t_queue));
  if (queue == NULL)
    return NULL;
  return queue;
}

bool init_queue_mutexs_conds(t_representer *representer) {
  if (pthread_mutex_init(&representer->queue->mutex_queue, NULL) != 0) 
    return false;
  return true;
}
