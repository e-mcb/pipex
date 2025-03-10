LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -g

SRC = pipex.c path_and_exec.c split2.c utils.c bool_utils.c
OBJ = $(SRC:.c=.o)

BONUS_SRC = heredoc_utils.c path_and_exec.c pipex_bonus.c utils.c split2.c bool_utils.c
BONUS_OBJ = $(BONUS_SRC:.c=.o)

NAME = pipex
BONUS_NAME = pipex_bonus

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BONUS_OBJ) $(LIBFT)
	$(CC) $(BONUS_OBJ) -L$(LIBFT_DIR) -lft -o $(BONUS_NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re
