LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC = pipex.c
OBJ = $(SRC:.c=.o)

NAME = pipex

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -I$(LIBFT_DIR) -c $(SRC)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)

fclean: clean 
	rm -rf $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all
