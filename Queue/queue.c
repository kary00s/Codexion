#include "../codexion.h"

static t_queue *queue_allocater();


bool init_queue(t_representer *representer) {
  t_queue *queue;
  
  queue = queue_allocater();
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

static t_queue *queue_allocater()
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
