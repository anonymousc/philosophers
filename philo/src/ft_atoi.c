#include "philo.h"
#include <unistd.h>

static int ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}
static int ft_isspace(char c)
{
	return((c >= 9 && c <= 13) || c == 32);
}
static int ft_isalpha(char c)
{
	return(!(c >= '0' && c <= '9'));
}

long ft_atoi(char *str)
{
	long long res;

	res = 0;
	while (str[res])
	{
		if(ft_isalpha(str[res]))
			return (0);
		res++;
	}
	res = 0;
	while (str && ft_isspace(*str))
		str++;
	if(*str == '+')
		str++;
	while (*str && ft_isdigit(*str))	
	{
		if(res > INT_MAX)
			return (write(2, "trying Some forbidden integer range\n", 37), 0);
		res = (res * 10) + (*str)  - '0';
		str++;
	}	
	return (res);
}
long get_time_m ()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); 
}

int get_time_micro ()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000) + (tv.tv_usec)); 
}

void ft_uslep (size_t time)
{
	size_t start = get_time_m();
	while (get_time_m() - start < time)
		usleep(500);		
}