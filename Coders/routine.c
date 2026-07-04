#include "../codexion.h"
#include <pthread.h>

bool wait_for_simulation_to_start(t_coder *coder);
static void compiling(t_coder *coder, t_queue *queue);
static void debuging(t_coder *coder);
static void refactoring(t_coder *coder);

static void compiling(t_coder *coder, t_queue *queue) {

  insert_coder_in_queue(coder, coder->queue);
  coder_waiting_dongles(coder);
  gettimeofday(&coder->last_compile, NULL);

  add_to_number_required_compilation(coder->representer);
  print_action(coder);
  action_simulator(coder, coder->coder_state);
  
  drop_both_dongles(coder);
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  coder->coder_state = DEBUGING;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
}

void add_to_number_required_compilation(t_representer *representer)
{
  pthread_mutex_lock(&representer->required_numbers_compilation_m_c.mutex);
  representer->required_numbers_compilation++;
  pthread_mutex_unlock(&representer->required_numbers_compilation_m_c.mutex);
}

static void debuging(t_coder *coder) {
  print_action(coder);
  action_simulator(coder, coder->coder_state);

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  coder->coder_state = REFACTORING;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
}

static void refactoring(t_coder *coder) {
  print_action(coder);
  action_simulator(coder, coder->coder_state);

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  coder->coder_state = WAITING;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
}

void action_simulator(t_coder *coder, t_coder_state state) {
  t_timespec time_spec;
  unsigned long time_action;
  unsigned long right_now;
  right_now = get_time_ms();
  pthread_mutex_lock(&coder->mutex_cond.mutex);

  if (state == COMPILING) {
    time_action = right_now + coder->config->time_to_compile;
    ms_to_timespec(&time_spec, time_action);
    pthread_cond_timedwait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex,
                           &time_spec);
  } else if (state == DEBUGING) {
    time_action = right_now + coder->config->time_to_debug;
    ms_to_timespec(&time_spec, time_action);
    pthread_cond_timedwait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex,
                           &time_spec);
  } else if (state == REFACTORING) {
    time_action = right_now + coder->config->time_to_refactor;
    ms_to_timespec(&time_spec, time_action);
    pthread_cond_timedwait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex,
                           &time_spec);
  }

  pthread_mutex_unlock(&coder->mutex_cond.mutex);
}

void *routine_all_the_coders(void *arg) {
  t_coder *coder;
  coder = (t_coder *)arg;
  wait_for_simulation_to_start(coder);

  if (coder->coder_id % 2 != 0)
    usleep(2000);

  while (1) {
    if (are_one_of_coders_burnout(coder->representer))
    {
      printf("one of them burnouted\n");
      break;
    }
    // if (are_required_numbers_compilation_done(coder->representer))
    // {
    //   printf("All coders compiled cleanly\n");
    //   break;
    // }
    compiling(coder, coder->queue);
    debuging(coder);
    refactoring(coder);
  }
  return NULL;
}

bool are_required_numbers_compilation_done(t_representer *representer)
{
  bool is_done;
  is_done = false;
  pthread_mutex_lock(&representer->required_numbers_compilation_m_c.mutex);
  if (representer->required_numbers_compilation >= representer->config.number_of_compiles_required)
    is_done = true;
  pthread_mutex_unlock(&representer->required_numbers_compilation_m_c.mutex);
  return is_done;  
}

bool wait_for_simulation_to_start(t_coder *coder) {
  pthread_mutex_lock(&coder->ready_coders_counter_m_c->mutex);
  (*coder->ready_coders_counter)++;
  pthread_cond_broadcast(&coder->ready_coders_counter_m_c->cond);
  pthread_mutex_unlock(&coder->ready_coders_counter_m_c->mutex);

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == STARTING)
    pthread_cond_wait(&coder->mutex_cond.cond, &coder->mutex_cond.mutex);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return true;
}

void coders_joiner(t_representer *representer) {
  int i;
  i = 0;
  while (i < representer->config.number_of_coders) {
    pthread_join(representer->coders[i]->thread, NULL);
    i++;
  }
}

void print_action(t_coder *coder) {
  pthread_mutex_lock(coder->print_mutex);

  if (coder->coder_state == REFACTORING)
    printf("=====  3  =======> %d coder is refactoring\n", coder->coder_id);

  if (coder->coder_state == DEBUGING)
    printf("=====  2  =======> %d coder is debuging\n", coder->coder_id);

  if (coder->coder_state == COMPILING) {
    printf("=====  1  =======> %d coder is compiling\n", coder->coder_id);
    // printf("%d has taken a dongle\n", coder->coder_id);
    // printf("%d has taken a dongle\n", coder->coder_id);
  }

  pthread_mutex_unlock(coder->print_mutex);
}
