NAME = philo

CFLAGS = -Wextra -Wall -Werror -I./lib/ #-g3 -fsanitize=address

CC = cc

src = src/ft_atoi.c main.c

obj = $(src:%.c=%.o)

all : $(NAME)

$(NAME) : $(obj) 
	$(CC) $(CFLAGS) $(obj) -pthread -o $(NAME)
clean :
	rm -rf $(obj)
fclean : clean
	rm -rf $(NAME)
re : fclean all

.SECONDARY : $(obj)