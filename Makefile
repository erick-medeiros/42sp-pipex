NAME = pipex

LIBFT_DIR = libft/
LIBFT = libft/libft.a

INC_DIR = include/
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -I $(LIBFT_DIR) -I $(INC_DIR) -g
LIBFLAGS = -lft
CC = cc
RM = rm -fr

SRC_DIR = src/
OBJ_DIR = obj/
BIN_DIR = bin/
REQUIRED_DIRS = $(OBJ_DIR) $(BIN_DIR)

BIN_MAND = $(BIN_DIR)$(NAME)
BIN_BONUS = $(BIN_MAND)_bonus

FILES = main.c
FILES += pipex.c
FILES += child_process.c
FILES += exit.c

SRC = $(addprefix $(SRC_DIR), $(FILES))
OBJ = $(addprefix $(OBJ_DIR), $(FILES:.c=.o))

FILES_BONUS = main_bonus.c
FILES_BONUS += pipex.c
FILES_BONUS += child_process.c
FILES_BONUS += here_doc_bonus.c
FILES_BONUS += exit.c

SRC_BONUS = $(addprefix $(SRC_DIR), $(FILES_BONUS))
OBJ_BONUS = $(addprefix $(OBJ_DIR), $(FILES_BONUS:.c=.o))

all: $(NAME)

$(REQUIRED_DIRS):
	mkdir -p $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(REQUIRED_DIRS) $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -o $(BIN_MAND) $(OBJ) -L $(LIBFT_DIR) $(LIBFLAGS)
	@cp $(BIN_MAND) $(NAME)

$(BIN_BONUS): $(REQUIRED_DIRS) $(OBJ_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(BIN_BONUS) $(OBJ_BONUS) -L $(LIBFT_DIR) $(LIBFLAGS)
	@cp $(BIN_BONUS) $(NAME)

clean:
	$(RM) $(OBJ)
	$(RM) $(OBJ_BONUS)

fclean: clean
	$(RM) $(NAME) $(BIN_MAND) $(BIN_BONUS)

re: fclean all

bonus: $(BIN_BONUS)

rebonus: fclean bonus

norm:
	@clear
	@norminette | grep Error || true

external:
	nm -un $(NAME) | grep -v w | grep -v __ | cut -d "@" -f1 | cut -c20-

test:
	./pipex files/infile "cat" "wc -l" files/outfile

test_bonus:
	./pipex files/infile "cat" "grep Ubuntu" "wc -l" files/outfile

leaks:
	@clear
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	./pipex files/infile "cat" "wc -l" files/outfile

.PHONY: all clean fclean re bonus rebonus norm external test leaks
