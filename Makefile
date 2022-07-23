NAME		= libftprintf.a
LIBFT_DIR	= libft
LIBFT		= libft.a

INC			= $(LIBFT_DIR)
SRCS		= ft_printf.c ft_write_chr.c ft_write_num.c
OBJS		= $(SRCS:.c=.o)
RM			= rm -f
CC			= gcc
CFLAGS		= -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJS)
	make all -C $(LIBFT_DIR)/
	cp $(LIBFT_DIR)/$(LIBFT) $(NAME)
	ar -rcus $(NAME) $(OBJS)

.c.o :
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@

clean :
	$(RM) $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean : clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re : fclean all

.PHONY: all bonus clean fclean re
