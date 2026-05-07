#include"codexion.h"

void exit_all(char *message)
{
    printf("%s\n", message);
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
    while(str[i])
    {
        if (((str[i] >= '0') && (str[i] <= '9')) 
    && (res < 9223372036854775807))
            res = 10 * res + str[i] - 48;
    
        if(str[i] == '-')
        exit_all("Args can't be negative");
        
        i++;
    }
    return res;
}

t_config parser(int ac, char **args)
{
    t_config config;

    if (ac == 8)
    {
        config.number_of_coders = ft_atol(args[1]);
        config.time_to_debug = ft_atol(args[4]);
        config.time_to_burnout = ft_atol(args[2]);
        config.time_to_compile = ft_atol(args[3]);
        config.time_to_refactor = ft_atol(args[5]);
        config.number_of_compiles_required = ft_atol(args[6]);
        config.dongle_cooldown = ft_atol(args[7]);
        // representer->config.scheduler = 
    }
    else
        exit_all("| ERROR |");
    return (config);
}