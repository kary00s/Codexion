

#include"codexion.h"
static void drop_dongle(t_dongle *dongle);
static void hold_dongle(t_dongle *dongle);


bool hold_both_dongles(t_coder *coder)
{

	hold_dongle(coder->left_dongle);
	hold_dongle(coder->right_dongle);

    return true;
}

void drop_both_dongles(t_coder *coder)
{
	
    drop_dongle(coder->right_dongle);
	drop_dongle(coder->left_dongle);
}

static void hold_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = false;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}


static void drop_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_m_c.mutex);
    dongle->is_available = true;
    dongle->last_reste =  get_time_ms();
    dongle->is_cold = false;
    pthread_mutex_unlock(&dongle->dongle_m_c.mutex);
}
