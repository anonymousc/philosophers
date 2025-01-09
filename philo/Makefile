NAME = philo

CFLAGS = -Wextra -Wall -Werror -I./lib/ -lpthread #-fsanitize=thread

CC = cc

src = src/ft_atoi.c main.c

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