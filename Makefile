LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -fPIE -g

SRC = pipex.c pathfinder.c
OBJ = $(SRC:.c=.o)

SRC_BONUS = pipex_bonus.c pathfinder_bonus.c utils_bonus.c
OBJ_BONUS = ${SRC_BONUS:.c=.o}

NAME = pipex
NAME_BONUS = pipex_bonus

all: $(NAME)


$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

bonus : ${NAME_BONUS}

$(NAME_BONUS): $(OBJ_BONUS) $(LIBFT)
	$(CC) $(OBJ_BONUS) -L$(LIBFT_DIR) -lft -o $(NAME_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ) ${OBJ_BONUS}

fclean: clean
	rm -f $(NAME) 
	make -C $(LIBFT_DIR) fclean

re: fclean all
