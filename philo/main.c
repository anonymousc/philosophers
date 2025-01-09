#include "philo.h"
#include <signal.h>

bool 	check_death(t_ph *ph)
{
	if (get_time_m() - ph->last_time_eat >= ph->tbla->time_to_die)
		return true;
	return false;
}

void	*monitor(void *data)
{
	t_tbla *hh;
	int i;

	hh = data;
	while (hh->num_ready_to_sync != hh->num_of_philos)
		continue;
	while  (!hh->end_of_sim)
	{
		ft_uslep(hh->time_to_die);
		i = 0;
		while (!hh->end_of_sim && i < hh->num_of_philos)
		{
			if(check_death(&hh->philos[i]) && !hh->end_of_sim)
			{
				hh->end_of_sim = check_death(&hh->philos[i]);
				printf("%ld %d died\n", get_time_m() - hh->start_of_sim ,hh->philos[i].id);
			}
			i++;
		}
	}
	return (NULL);
}

int	input_validation(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (ft_atoi(av[i]) <= 0)
			return (av = NULL,2);
		i++;
	}
	return (0);
}

int	parser(char **av, int ac)
{
	if (ac < 4 || ac > 7)
		return (10);
	return (input_validation(av));
}

int	ft_handle_parse_error(char **av, int ac)
{
	int philos;

	philos = parser(av, ac);
	if (philos == 10 || philos == 2)
		return (write(2, "Invalid Args\n", 13), -1);
	return (philos);
}

void	ft_sync(t_tbla *tbla)
{
	while (tbla->num_ready_to_sync != tbla->num_of_philos);
} 

void	think(t_ph *philo)
{
	if(!philo->tbla->end_of_sim)
	{
		printf ("%ld %d is thinking\n", get_time_m() - philo->tbla->start_of_sim ,  philo->id);
		if(philo->tbla->num_of_philos % 2)
			ft_uslep(((philo->tbla->time_to_eat * 2) - philo->tbla->time_to_sleep));
		else
			return ;
	}
}

void	eat(t_ph *philo, atomic_long *last_meal)
{
	pthread_mutex_lock(&philo->fork_1->fork);
	if(!philo->tbla->end_of_sim)
		printf ("%ld %d has taken a fork\n", get_time_m() - philo->tbla->start_of_sim ,  philo->id);
	pthread_mutex_lock(&philo->fork_2->fork);
	if(!philo->tbla->end_of_sim)
		printf ("%ld %d has taken a fork\n", get_time_m() - philo->tbla->start_of_sim,  philo->id);
	*last_meal = get_time_m();
	if(!philo->tbla->end_of_sim)
		printf ("%ld %d is eating\n", get_time_m() - philo->tbla->start_of_sim,  philo->id);
	philo->meals_eaten++;
	if(!philo->tbla->end_of_sim)
		ft_uslep(philo->tbla->time_to_eat);
	pthread_mutex_unlock(&philo->fork_2->fork);
	pthread_mutex_unlock(&philo->fork_1->fork);
}
void sleep_p(t_ph *philo)
{
	if(!philo->tbla->end_of_sim)
	{
		printf ("%ld %d is sleeping\n",  get_time_m() - philo->tbla->start_of_sim,  philo->id);
		ft_uslep(philo->tbla->time_to_sleep);
	}
}
void *dinner(void *data)
{
	t_ph *philo;

	philo = (t_ph*)data;
	philo->tbla->start_of_sim = get_time_m();
	while (!philo->tbla->ready);
	philo->meals_eaten = 0;
	philo->ready = true;
	philo->tbla->num_ready_to_sync++;
	while (1)
	{
		if(philo->tbla->end_of_sim == true)
			return (NULL);
		eat(philo, &philo->last_time_eat);
		if(philo->meals_eaten == philo->tbla->num_of_meals)
			return (NULL);
		sleep_p(philo);
		think(philo);
	}
	return NULL;
}
void *single_threaded(void *data)
{
	t_ph *philo;
	philo = (t_ph *)data;
	while (1)
	{
		pthread_mutex_lock(&philo->fork_1->fork);
		printf ("%ld %d has taken a fork\n", get_time_m() - philo->tbla->start_of_sim ,  1);
		pthread_mutex_unlock(&philo->fork_1->fork);
		ft_uslep(philo->tbla->time_to_die);
		printf("%ld %d died\n", get_time_m() - philo->tbla->start_of_sim ,philo->tbla->philos[0].id);
		return NULL;
	}
}
int sim_dinner(t_tbla *tbla)
{
	int i;
	i = 0;
	if(tbla->num_of_philos == 1)
	{
		tbla->start_of_sim = get_time_m();
		pthread_create(&tbla->philos[0].ph_id, NULL, single_threaded , tbla->philos);
		pthread_join(tbla->philos[0].ph_id, NULL);
	}
	else 
	{
		while (i < tbla->num_of_philos)
		{
			pthread_create(&tbla->philos[i].ph_id, NULL, dinner , tbla->philos + i);
			i++;
		}
		pthread_create(&tbla->monitor, NULL, monitor , tbla);
		tbla->ready = true;
		i = 0;
		while (i < tbla->num_of_philos)
		{
			pthread_join(tbla->philos[i].ph_id, NULL);
			i++;
		}
		pthread_join(tbla->monitor, NULL);
	}
	return 0;
}

void take_forks(t_tbla *tbla, int pos)
{
	tbla->philos[pos].fork_1 = &tbla->forks[pos];
	tbla->philos[pos].fork_2 = &tbla->forks[(pos + 1) % tbla->num_of_philos];

	if (tbla->philos[pos].id % 2 == 0)
	{
		tbla->philos[pos].fork_1 = &tbla->forks[(pos + 1) % tbla->num_of_philos];
		tbla->philos[pos].fork_2 = &tbla->forks[pos];
	}
}

void init_table(t_tbla *tbla, char **av)
{
	tbla->num_of_philos = ft_atoi(av[1]);
	tbla->time_to_die = ft_atoi(av[2]);
	tbla->time_to_eat = ft_atoi(av[3]);
	tbla->time_to_sleep = ft_atoi(av[4]);
	tbla->end_of_sim = false;
	tbla->ready = false;
	if (av[5])
		tbla->num_of_meals = ft_atoi(av[5]);
	tbla->num_ready_to_sync = 0;
	tbla->philos = malloc(sizeof(t_ph) * tbla->num_of_philos);
	tbla->forks = malloc(sizeof(t_fork) * tbla->num_of_philos);

	int i = 0;
	while (i < tbla->num_of_philos)
	{
		pthread_mutex_init(&tbla->forks[i].fork, NULL);
		tbla->forks[i].id = i;
		i++;
	}
	 i = 0;
	while (i < tbla->num_of_philos)
	{
		tbla->philos[i].id = i + 1;
		tbla->philos[i].tbla = tbla;
		tbla->philos[i].is_full = false;
		tbla->philos[i].ready = false;
		take_forks(tbla, i);
		i++;
 	}
}

int	main(int ac, char **av)
{
	t_tbla *tbla =  malloc (sizeof(t_tbla));

	if (ft_handle_parse_error(av, ac) == -1)
		return (1);
	init_table(tbla, av);
	sim_dinner(tbla);
	// clean(tbla);
} 