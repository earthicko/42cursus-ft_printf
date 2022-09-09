NAME			= libftprintf.a
LIBFT_DIR		= libft
LIBFT			= libft.a
INC_DIR_LIBFT	= -I $(LIBFT_DIR)

INC_DIR			= -I . $(INC_DIR_LIBFT) -I include
SRC_DIR			= ./src/

INC_DIR_BONUS	= -I . $(INC_DIR_LIBFT) -I include_bonus
SRC_DIR_BONUS	= ./src_bonus/

SRCNAME			= \
				ft_printf \
				fwrite_chr fwrite_num fwrite_ptr \
				fwrite_utils ft_str_utils ft_numlen \
				parse_format parse_format_utils t_conv \
				create_str create_str_utils

SRCNAME_BONUS	= \
				ft_printf_bonus \
				fwrite_chr_bonus fwrite_num_bonus fwrite_ptr_bonus \
				fwrite_utils_bonus ft_str_utils_bonus ft_numlen_bonus \
				parse_format_bonus parse_format_utils_bonus t_conv_bonus \
				create_str_bonus create_str_utils_bonus

SRC				= $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRCNAME)))
OBJ				= $(addprefix $(SRC_DIR), $(addsuffix .o, $(SRCNAME)))

SRC_BONUS		= $(addprefix $(SRC_DIR_BONUS), $(addsuffix .c, $(SRCNAME_BONUS)))
OBJ_BONUS		= $(addprefix $(SRC_DIR_BONUS), $(addsuffix .o, $(SRCNAME_BONUS)))

RM				= rm -f
CC				= gcc
CFLAGS			= -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJ)
	make all -C $(LIBFT_DIR)/
	cp $(LIBFT_DIR)/$(LIBFT) $(NAME)
	ar -rcus $(NAME) $(OBJ)

bonus : $(OBJ_BONUS)
	make all -C $(LIBFT_DIR)/
	cp $(LIBFT_DIR)/$(LIBFT) $(NAME)
	ar -rcus $(NAME) $(OBJ_BONUS)

$(SRC_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

$(SRC_DIR_BONUS)%.o: $(SRC_DIR_BONUS)%.c
	$(CC) $(CFLAGS) $(INC_DIR_BONUS) -c $< -o $@

clean :
	$(RM) $(OBJ) $(OBJ_BONUS)
	make clean -C $(LIBFT_DIR)

fclean : clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

re :
	make fclean
	make all

.PHONY: all bonus clean fclean re
