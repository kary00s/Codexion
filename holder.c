#include "codexion.h"


static void hold_the_right_dongle(t_coder *coder)
{
    pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
    printf("%d %d has taken a dongle\n", coder->right_dongle->dongle_id, coder->coder_id);
    coder->right_dongle->is_available = false;
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}

static void hold_the_left_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
	(coder->left_dongle->is_available) = false;
	printf("%d %d has taken a dongle\n", coder->left_dongle->dongle_id, coder->coder_id);
	pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
}


void hold_both_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	hold_the_left_dongle(coder);	
	hold_the_right_dongle(coder);
	pthread_mutex_unlock(&coder->mutex);
}