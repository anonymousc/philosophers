#include "philo.h"


int	input_validation(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (ft_atoi(av[i]) == 0)
			return (av = NULL,2);
		if(ft_atoi(av[i]) <= -1)
			return (printf("Positive value !\n"),av = NULL,3);
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
	if(philos == 2)
		return (-1);
	if (philos == 10 || philos == 3)
		return (write(2, "Invalid Args\n", 13), -1);
	return (philos);
}

void ft_sync(t_tbla *tbla)
{
	while (tbla->num_ready_to_sync != tbla->num_of_philos);
}

void think(t_ph *philo)
{
	printf ("%d %d is thinking\n", get_time_m() - philo->tbla->start_of_sim ,  philo->id);
}

void eat (t_ph *philo)
{
	// pthread_mutex_lock(&philo->fork_1->fork);
	printf ("%d %d has taken a fork\n", get_time_m() - philo->tbla->start_of_sim ,  philo->id);
	// pthread_mutex_lock(&philo->fork_2->fork);
	printf ("%d %d has taken a fork\n", get_time_m() - philo->tbla->start_of_sim,  philo->id);
	philo->last_time_eat = get_time_m();
	printf ("%d %d is eating\n", get_time_m() - philo->tbla->start_of_sim,  philo->id);
	philo->meals_eaten++;
	ft_uslep(philo->tbla->time_to_eat * 1000);
	// pthread_mutex_unlock(&philo->fork_1->fork);
	// pthread_mutex_unlock(&philo->fork_2->fork);
}

void *dinner(void *data)
{
	t_ph *philo;

	philo = (t_ph*)data;
	philo->ready = true;
	philo->tbla->num_ready_to_sync++;
	ft_sync(philo->tbla);
	philo->tbla->start_of_sim = get_time_m();
	while (!philo->tbla->to_die)
	{
		while (1)
		{
			eat(philo);
			printf ("%d %d is sleeping\n",  get_time_m() - philo->tbla->start_of_sim,  philo->id);
			ft_uslep(philo->tbla->time_to_sleep * 1000);
			think(philo);
			if(philo->meals_eaten == philo->tbla->num_of_meals)
				return NULL;
		}
		ft_uslep(philo->tbla->time_to_die * 1000);
		philo->tbla->to_die = 1;
		printf("%d %d died\n",get_time_m() - philo->tbla->philos->last_time_eat , philo->tbla->philos->id);
		if(philo->tbla->to_die == 1)
			return NULL;
	}
	return NULL;
}

int sim_dinner(t_tbla *tbla)
{
	int i = 0;
	while (i < tbla->num_of_philos)
	{
		pthread_create(&tbla->philos[i].ph_id, NULL, dinner , tbla->philos + i);
		i++;
	}
	i = 0;
	while (i < tbla->num_of_philos)
	{
		pthread_join(tbla->philos[i].ph_id, NULL);
		i++;
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
	tbla->to_exit = 0;
	tbla->to_die = 0;
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
	t_tbla *tbla =  malloc (sizeof(tbla));

	if (ft_handle_parse_error(av, ac) != 0)
		return (1);
	init_table(tbla, av);
	sim_dinner(tbla);
	// clean(tbla);
} 