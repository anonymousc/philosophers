#include "philo.h"
#include <signal.h>

bool 	check_death(t_ph *ph)
{
	if (get_time_m() - ph->last_time_eat >= ph->table->time_to_die)
		return true;
	return false;
}

void	*monitor(void *data)
{
	t_table *hh;
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
			if (hh->philos->meals_eaten == hh->num_of_meals)
				return NULL;
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

void	ft_sync(t_table *table)
{
	while (table->num_ready_to_sync != table->num_of_philos);
} 

void	think(t_ph *philo)
{
	if(!philo->table->end_of_sim)
	{
		printf ("%ld %d is thinking\n", get_time_m() - philo->table->start_of_sim ,  philo->id);
		ft_uslep(60);
	}
}

void	eat(t_ph *philo, atomic_long *last_meal)
{
	pthread_mutex_lock(&philo->fork_1->fork);
	if(!philo->table->end_of_sim)
		printf ("%ld %d has taken a fork\n", get_time_m() - philo->table->start_of_sim ,  philo->id);
	pthread_mutex_lock(&philo->fork_2->fork);
	if(!philo->table->end_of_sim)
		printf ("%ld %d has taken a fork\n", get_time_m() - philo->table->start_of_sim,  philo->id);
	*last_meal = get_time_m();
	if(!philo->table->end_of_sim)
		printf ("%ld %d is eating\n", get_time_m() - philo->table->start_of_sim,  philo->id);
	philo->meals_eaten++;
	if(!philo->table->end_of_sim)
	{
		while (get_time_m() - philo->table->start_of_sim <= philo->table->time_to_eat)
			ft_uslep(60);
	}
	pthread_mutex_unlock(&philo->fork_2->fork);
	pthread_mutex_unlock(&philo->fork_1->fork);
}
void sleep_p(t_ph *philo)
{
	if(!philo->table->end_of_sim)
	{
		printf ("%ld %d is sleeping\n",  get_time_m() - philo->table->start_of_sim,  philo->id);
		ft_uslep(philo->table->time_to_sleep);
	}
}
void *dinner(void *data)
{
	t_ph *philo;

	philo = (t_ph*)data;
	philo->table->start_of_sim = get_time_m();
	philo->meals_eaten = 0;
	philo->table->num_ready_to_sync++;
	while (1)
	{
		if(philo->table->end_of_sim == true)
			return (NULL);
		eat(philo, &philo->last_time_eat);
		if(philo->meals_eaten == philo->table->num_of_meals)
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
		printf ("%ld %d has taken a fork\n", get_time_m() - philo->table->start_of_sim ,  1);
		pthread_mutex_unlock(&philo->fork_1->fork);
		ft_uslep(philo->table->time_to_die);
		printf("%ld %d died\n", get_time_m() - philo->table->start_of_sim ,philo->table->philos[0].id);
		return NULL;
	}
}
int sim_dinner(t_table *table)
{
	int i;
	i = 0;
	if(table->num_of_philos == 1)
	{
		table->start_of_sim = get_time_m();
		pthread_create(&table->philos[0].ph_id, NULL, single_threaded , table->philos);
		pthread_join(table->philos[0].ph_id, NULL);
	}
	else 
	{
		while (i < table->num_of_philos)
		{
			pthread_create(&table->philos[i].ph_id, NULL, dinner , table->philos + i);
			i++;
		}
		pthread_create(&table->monitor, NULL, monitor , table);
		i = 0;
		while (i < table->num_of_philos)
		{
			pthread_join(table->philos[i].ph_id, NULL);
			i++;
		}
		pthread_join(table->monitor, NULL);
	}
	return 0;
}

void take_forks(t_table *table, int pos)
{
	table->philos[pos].fork_1 = &table->forks[pos];
	table->philos[pos].fork_2 = &table->forks[(pos + 1) % table->num_of_philos];

	if (table->philos[pos].id % 2 == 0)
	{
		table->philos[pos].fork_1 = &table->forks[(pos + 1) % table->num_of_philos];
		table->philos[pos].fork_2 = &table->forks[pos];
	}
}

void init_table(t_table *table, char **av)
{
	int i;

	i = 0;
	table->num_of_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->end_of_sim = false;
	if (av[5])
		table->num_of_meals = ft_atoi(av[5]);
	table->num_ready_to_sync = 0;
	table->philos = malloc(sizeof(t_ph) * table->num_of_philos);
	table->forks = malloc(sizeof(t_fork) * table->num_of_philos);

	while (i < table->num_of_philos)
	{
		pthread_mutex_init(&table->forks[i].fork, NULL);
		table->forks[i].id = i;
		i++;
	}
	i = 0;
	while (i < table->num_of_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		table->philos[i].is_full = false;
		take_forks(table, i);
		i++;
 	}
}

int	main(int ac, char **av)
{
	t_table *table =  malloc (sizeof(t_table));

	if (ft_handle_parse_error(av, ac) == -1)
		return (1);
	init_table(table, av);
	sim_dinner(table);
	// clean(table);
} 