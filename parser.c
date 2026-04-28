#include"codexion.h"
void exit_all()
{
    printf(" | Exit Process |\n");
    exit(EXIT_FAILURE);
}
static long ft_atol(char *str)
{
    int i ;
    i = 0;
    long res;
    res = 0;
    if (!str)
        return 42;

    while((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
        i++;
    while((((str[i] >= '0') && (str[i] <= '9')) 
            && (res < 9223372036854775807)))
    {
        res = 10 * res + str[i] - 48;
        i++;
    }
    if(str[i] != '\0')
        exit_all();
    return res;
}
void parser(t_scene *scene,int ac, char **args)
{
    if (ac == 8)
    {
        scene->number_of_coders = ft_atol(args[1]);
        scene->time_to_burnout = ft_atol(args[2]);
        scene->time_to_compile = ft_atol(args[3]);
        scene->time_to_debug = ft_atol(args[4]);
        scene->time_to_refactor = ft_atol(args[5]);
        scene->number_of_compiles_required = ft_atol(args[6]);
        scene->dongle_cooldown = ft_atol(args[7]);
        // printf("=======\n");
    }
    else
        exit_all();
}