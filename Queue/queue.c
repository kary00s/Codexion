#include "../codexion.h"
static void shift_queue_down_fifo(t_queue *queue, int i);
static void shift_queue_down_edf(t_queue *queue, int i);

// void shift_queue_up(t_queue *queue, int index) {
//   int parent;

//   pthread_mutex_lock(&queue->mutex_queue);
//   while (index > 0) 
//   {
//     parent = (index - 1) / 2;
//     if (&queue->coders[parent]->last_compile <=
//         &queue->coders[index]->last_compile)
//       break;
//     swap_coders(&queue->coders[parent], &queue->coders[index]);
//     index = parent;
//   }
//   pthread_mutex_unlock(&queue->mutex_queue);
// }

void swap_coders(t_coder **parent_coder, t_coder **child_coder) {
  t_coder *swp;
  swp = *parent_coder;
  *parent_coder = *child_coder;
  *child_coder = swp;
}

static void shift_queue_down_edf(t_queue *queue, int i) 
{
  int right;
  int left;
  int smallest;
  unsigned long left_time;
  unsigned long right_time;
  unsigned long smallest_time;
  
  swap_coders(&queue->coders[0], &queue->coders[i]);
  swap_coders(&queue->coders[0], &queue->coders[queue->size-1]);

  while (1)
  {
    right = i * 2 + 2;
    left = i * 2 + 1;
    smallest = i;

    left_time = timeval_to_ms(queue->coders[left]->last_compile);
    right_time = timeval_to_ms(queue->coders[left]->last_compile);
    smallest_time = timeval_to_ms(queue->coders[left]->last_compile);


    if (left < queue->size && (left_time < smallest_time))
      smallest = left;

    if (right < queue->size && (right_time < smallest_time))
      smallest = right;

    if (smallest == i)
      break;

    swap_coders(&queue->coders[i], &queue->coders[smallest]);
    i = smallest;
  }
}


bool pop_coder_from_queue(t_representer *representer, int i) {
  t_queue *queue;
  queue = representer->queue;
    if (representer->queue->coders[i] == NULL)
      return false;

  if (representer->config.scheduler == FIFO)
    shift_queue_down_fifo(queue, i);
  else if (representer->config.scheduler == EDF)
    shift_queue_down_edf(queue, i);

  queue->size--;

  return true;
}

static void shift_queue_down_fifo(t_queue *queue, int i) {

  while (i < queue->size - 1) {
    queue->coders[i] = queue->coders[i + 1];
    i++;
  }
}

void insert_coder_in_queue(t_coder *coder, t_queue *queue) {
  pthread_mutex_lock(&coder->queue->mutex_queue);
  queue->coders[queue->size] = coder;
  queue->size++;
  pthread_mutex_unlock(&coder->queue->mutex_queue);
}

bool init_queue(t_representer *representer) {
  t_queue *queue;

  queue = malloc(sizeof(t_queue));
  if (queue == NULL)
  {
    representer_mutexes_destroyer(representer);
    return false;
  }
  queue->capacity = representer->config.number_of_coders;
  queue->size = 0;
  queue->coders = coders_allocater(queue->capacity);
  if (queue->coders == NULL) 
  {
    representer_mutexes_destroyer(representer);
    free(queue);
    return false;
  }
  representer->queue = queue;
  if (!init_queue_mutexs_conds(representer))
    return false;
  return true;
}

bool init_queue_mutexs_conds(t_representer *representer) {
  if (pthread_mutex_init(&representer->queue->mutex_queue, NULL) != 0) 
  {
    free(representer->queue); // frree queue struct
    pthread_mutex_destroy(&representer->finished_coders_mutex);
    pthread_mutex_destroy(&representer->print_mutex);
    destroy_mutex_cond(&representer->ready_coders_counter_m_c);   
    return false;
  }
  return true;
}

void clean_queue(t_queue *queue)
{
  free_coders(queue->coders);
  free(queue);
  pthread_mutex_destroy(&queue->mutex_queue);
}