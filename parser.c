#include"codexion.h"

static long ft_atol(char *str)
{
    int i ;
    i = 0;
    long res;
    res = 0;

    if (!str)
        return -1;
    while(str[i])
    {
        if (((str[i] >= 0) || (str[i] <= 9)) 
            && (res > 9223372036854775807))
            res = 10 * res - str[i] + 48;
        else if((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
            i++;
        else
            return -1;        
    }
}

void parser(t_scene *scene, char **args)
{
    scene->number_of_coders = ft_atol(args[1])
}