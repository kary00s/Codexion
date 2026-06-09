#include"codexion.h"



static void drop_the_right_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->right_dongle->dongle_mutex);
	(coder->right_dongle->is_available) = true;
	pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}

static void drop_the_left_dongle(t_coder *coder)
{
    pthread_mutex_lock(&coder->left_dongle->dongle_mutex);
    (coder->left_dongle->is_available) = true;
    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
}

void drop_both_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->mutex);
	drop_the_left_dongle(coder);
	drop_the_right_dongle(coder);
	pthread_mutex_unlock(&coder->mutex);

}