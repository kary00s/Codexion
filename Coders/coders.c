
#include "../codexion.h"

static void initialize_coders_struct(t_representer *representer);

t_coder **coders_allocater(int number_of_coders) 
{
  t_coder **coders_list;
  int i;

  i = 0;
  coders_list = (t_coder **)malloc(sizeof(t_coder *) * number_of_coders);
  if (!coders_list)
    return (NULL);
  while (i < number_of_coders) {
    coders_list[i] = (t_coder *)malloc(sizeof(t_coder));
    if (!coders_list[i])
    {
      free_coders(coders_list, i);
      return NULL;
    }
    i++;
  }
  return (coders_list);
}

bool init_coders(t_representer *representer) 
{
  int number_of_coders;

  number_of_coders = representer->config.number_of_coders;
  representer->coders = coders_allocater(number_of_coders);
  if (!representer->coders)
    return false;

  initialize_coders_struct(representer);
  if (!init_coders_mutexes_conds(representer->coders, number_of_coders))
  {
    free_coders(representer->coders, representer->config.number_of_coders);
    return false;
  }
  return true;
}

static void initialize_coders_struct(t_representer *representer) {
  int i;
  t_coder **coders;

  i = 0;
  coders = representer->coders;
  while (i < representer->config.number_of_coders) {
    coders[i]->config = &representer->config;
    coders[i]->coder_id = i;
    coders[i]->queue = representer->queue;
    coders[i]->print_mutex = &representer->print_mutex;
    coders[i]->ready_coders_counter = &representer->ready_coders_counter;
    coders[i]->ready_coders_counter_m_c = &representer->ready_coders_counter_m_c;
    coders[i]->coder_state = START;
    coders[i]->numbers_compilation = 0;
    coders[i]->begining_time = &representer->begining_time;
    coders[i]->finished_coders_mutex = &representer->finished_coders_mutex;
    coders[i]->finished_coders = &representer->finshed_coders;
    coders[i]->is_burnout_mutex = &representer->is_burnout_mutex;
    coders[i]->is_burnout = &representer->is_burnout;
    i++;
  }
}

bool coders_creator(t_representer *representer) {
  int i = 0;

  while (i < representer->config.number_of_coders) {
    if (pthread_create(&representer->coders[i]->thread, NULL, routine_all_the_coders, representer->coders[i]) != 0)
    {
      exit_representation(representer);
      broadcast_coders_to_exit(representer, i);
      representer_mutexes_destroyer(representer);
      clean_initialize_representer_struct(representer);
      return false;
    }
    i++;
  }
  return true;
}
