#include "../codexion.h"

bool wait_for_simulation_to_start(t_coder *coder);
static bool compiling(t_coder *coder, t_queue *queue);
static bool debuging(t_coder *coder);
static bool refactoring(t_coder *coder);
static bool add_to_number_required_compilation(t_coder *coder);
static bool action_simulator(t_coder *coder, t_coder_state state);
static void change_numbers_required_for_representer(t_coder *coder);
static void add_coder_to_finished_coders(t_coder *coder);

static bool compiling(t_coder *coder, t_queue *queue) {
  insert_coder_in_queue(coder, coder->queue);
  if (!coder_waiting_dongles(coder))
    return false;
  if (!check_dongles_coldness(coder))
    return false;
  print_action(coder);
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  gettimeofday(&coder->last_compile, NULL);
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  add_coder_to_finished_coders(coder);
  if (!action_simulator(coder, coder->coder_state))
    return false;
  drop_both_dongles(coder);
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  coder->coder_state = DEBUGING;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return true;
}

static bool is_coder_finished(t_coder *coder) {
  bool is_finished;

  is_finished = false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  if (coder->config->number_of_compiles_required == coder->numbers_compilation)
    is_finished = true;
  coder->numbers_compilation++;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return is_finished;
}

static void add_coder_to_finished_coders(t_coder *coder) {
  bool is_finished;
  is_finished = is_coder_finished(coder);
  if (is_finished) {
    pthread_mutex_lock(coder->finished_coders_mutex);
    (*coder->finished_coders)++;
    pthread_mutex_unlock(coder->finished_coders_mutex);
  }
}

static bool add_to_number_required_compilation(t_coder *coder) {
  pthread_mutex_lock(&coder->numbers_compilation_m_c.mutex);
  if (coder->numbers_compilation == coder->config->number_of_compiles_required)
    return false;
  coder->numbers_compilation++;
  pthread_mutex_unlock(&coder->numbers_compilation_m_c.mutex);
  return true;
}

static bool debuging(t_coder *coder) {
  print_action(coder);
  if (!action_simulator(coder, coder->coder_state))
    return false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  coder->coder_state = REFACTORING;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return is_representation_works_well(coder->is_burnout_mutex,
                                      coder->is_burnout);
}

static bool refactoring(t_coder *coder) {
  print_action(coder);
  if (!action_simulator(coder, coder->coder_state))
    return false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  coder->coder_state = WAIT;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return is_representation_works_well(coder->is_burnout_mutex,
                                      coder->is_burnout);
}

bool wait(pthread_mutex_t *mutex, pthread_cond_t *cond, unsigned long time) {
  t_timespec time_spec;
  bool is_ok;

  is_ok = true;
  pthread_mutex_lock(mutex);
  ms_to_timespec(&time_spec, time);
  if (pthread_cond_timedwait(cond, mutex, &time_spec) != ETIMEDOUT)
    is_ok = false;
  pthread_mutex_unlock(mutex);
  return is_ok;
}

static bool action_simulator(t_coder *coder, t_coder_state state) {
  unsigned long time_action;
  unsigned long right_now;

  right_now = get_time_ms();

  time_action = right_now;
  if (state == COMPILING)
    time_action += coder->config->time_to_compile;
  else if (state == DEBUGING)
    time_action += coder->config->time_to_debug;
  else if (state == REFACTORING)
    time_action += coder->config->time_to_refactor;
  if (!wait(&coder->mutex_cond.mutex, &coder->mutex_cond.cond, time_action))
    return is_representation_works_well(coder->is_burnout_mutex,
                                        coder->is_burnout);
  return true;
}
static void sleep_odd_coders(t_coder *coder) {
  if (coder->coder_id % 2 == 0)
    usleep(500);
}

void *routine_all_the_coders(void *arg) {
  t_coder *coder;
  coder = (t_coder *)arg;
  wait_for_simulation_to_start(coder);
  sleep_odd_coders(coder);

  while (is_representation_works_well(coder->is_burnout_mutex,
                                      coder->is_burnout)) {
    if (!compiling(coder, coder->queue))
      break;
    if (!debuging(coder))
      break;
    if (!refactoring(coder))
      break;
  }
  printf("coder %d exit\n", coder->coder_id);
  return NULL;
}
static void change_numbers_required_for_representer(t_coder *coder) {}

bool wait_for_simulation_to_start(t_coder *coder) {
  pthread_mutex_lock(&coder->ready_coders_counter_m_c->mutex);
  (*coder->ready_coders_counter)++;
  pthread_cond_broadcast(&coder->ready_coders_counter_m_c->cond);
  pthread_mutex_unlock(&coder->ready_coders_counter_m_c->mutex);

  pthread_mutex_lock(&coder->mutex_cond.mutex);
  while (coder->coder_state == START)
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
  long time_elapsed;
  pthread_mutex_lock(coder->print_mutex);
  time_elapsed = time_elapsed_until_now(*coder->begining_time);

  if (coder->coder_state == REFACTORING)
    printf("%ld %d coder is refactoring\n", time_elapsed, coder->coder_id);

  if (coder->coder_state == DEBUGING)
    printf("%ld %d coder is debuging\n", time_elapsed, coder->coder_id);

  if (coder->coder_state == COMPILING) {
    printf("%ld %d coder is compiling\n", time_elapsed, coder->coder_id);
    // printf("%ld %d has taken a dongle\n",time_elapsed , coder->coder_id);
    // printf("%ld %d has taken a dongle\n",time_elapsed , coder->coder_id);
  }

  pthread_mutex_unlock(coder->print_mutex);
}
