#include"codexion.h"

void free_representer_struct(t_representer *representer)
{
    free_dongles(representer);
    free_coders(representer);
    free(representer);
}

void free_dongles(t_representer *representer)
{
    int i;

    i = 0;
    while((representer->dongles[i] != NULL) && ( i <= representer->config.number_of_coders))
        free(representer->dongles[i++]);
    free(representer->dongles);
}

void free_coders(t_representer *representer)
{
    int i;

    i = 0;
    while((representer->coders[i] != NULL) && ( i <= representer->config.number_of_coders))
        free(representer->coders[i++]);
    free(representer->coders);
}
