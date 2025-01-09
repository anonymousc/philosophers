NAME = philo

CFLAGS = -Wextra -Wall -Werror -I./lib/ -lpthread -fsanitize=address -g3

CC = cc

src = src/ft_atoi.c src/dinner_simulation.c src/exception.c src/init_all.c src/monitoring.c src/parsing.c src/philo_utils.c main.c

obj = $(src:%.c=%.o)

all : $(NAME)

$(NAME) : $(obj) 
	$(CC) $(CFLAGS) $(obj) -o $(NAME)
clean :
	rm -rf $(obj)
fclean : clean
	rm -rf $(NAME)
re : fclean all

.SECONDARY : $(obj)