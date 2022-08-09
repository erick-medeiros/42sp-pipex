NAME = pipex
LIBFT = libft/libft.a
LIBFT_DIR = libft/

CFLAGS = -Wall -Wextra -Werror
CFLAGS += -I $(LIBFT_DIR) -g
LIBFLAGS = -lft
CC = cc
RM = rm -fr

FILES = main.c

SRC_DIR = src/
OBJ_DIR = obj/
SRC = $(addprefix $(SRC_DIR), $(FILES))
OBJ = $(addprefix $(OBJ_DIR), $(FILES:.c=.o))

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJ_DIR) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L $(LIBFT_DIR) $(LIBFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

norm:
	@clear
	@norminette | grep Error || true

.PHONY: all clean fclean re norm