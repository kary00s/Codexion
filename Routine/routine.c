#include "../codexion.h"

bool wait_for_simulation_to_start(t_coder *coder);
static bool action_simulator(t_coder *coder, t_coder_state state);
static void sleep_odd_coders(t_coder *coder);
void add_coder_to_finished_coders(t_coder *coder);

void *routine_all_the_coders(void *arg) 
{
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
  return NULL;
}

static bool is_coder_finished(t_coder *coder) {
  bool is_finished;

  is_finished = false;
  pthread_mutex_lock(&coder->mutex_cond.mutex);
  if (coder->config->number_of_compiles_required == coder->numbers_compilation + 1)
    is_finished = true;
  coder->numbers_compilation++;
  pthread_mutex_unlock(&coder->mutex_cond.mutex);
  return is_finished;
}

void add_coder_to_finished_coders(t_coder *coder) 
{
  bool is_finished;
  is_finished = is_coder_finished(coder);
  if (is_finished) {
    pthread_mutex_lock(coder->finished_coders_mutex);
    (*coder->finished_coders)++;
    pthread_mutex_unlock(coder->finished_coders_mutex);
  }
}

static void sleep_odd_coders(t_coder *coder)
{
  if (coder->coder_id % 2 == 0)
    usleep(500);
}

bool wait_for_simulation_to_start(t_coder *coder)
{
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

void print_action(t_coder *coder)
{
  long time_elapsed;
  pthread_mutex_lock(coder->print_mutex);
  time_elapsed = time_elapsed_until_now(*coder->begining_time);

  if (coder->coder_state == REFACTORING)
    printf("%ld %d is refactoring\n", time_elapsed, coder->coder_id + 1);

  if (coder->coder_state == DEBUGING)
    printf("%ld %d is debuging\n", time_elapsed, coder->coder_id + 1);

  if (coder->coder_state == COMPILING) {
    printf("%ld %d is compiling\n", time_elapsed, coder->coder_id + 1);
    printf("%ld %d has taken a dongle\n",time_elapsed , coder->coder_id + 1);
    printf("%ld %d has taken a dongle\n",time_elapsed , coder->coder_id + 1);
  }

  if(coder->coder_state == EXIT)
    printf(">> %ld %d coder burnouted\n", time_elapsed, coder->coder_id);
    
  pthread_mutex_unlock(coder->print_mutex);
}
