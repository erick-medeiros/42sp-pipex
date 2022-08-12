NAME = pipex

LIBFT_DIR = libft/
LIBFT = libft/libft.a

INC_DIR = include/
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -I $(LIBFT_DIR) -I $(INC_DIR) -g
LIBFLAGS = -lft
CC = cc
RM = rm -fr

FILES = main.c
FILES += pipex.c
FILES += child_process.c
FILES += free.c

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

test: $(NAME)
	./$(NAME) files/infile "cat" "wc -l" files/outfile

leaks: $(NAME)
	@clear
	valgrind \
	--leak-check=full \
	./$(NAME) files/infile "cat" "wc -l" files/outfile

.PHONY: all clean fclean re norm test leaks