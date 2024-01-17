NAME := pipex
CC := gcc

SRC_DIR := ./source
INC_DIR := ./include
LIBFT_INC_DIR := ./libft/inc
BIN_DIR := ./bin

STATIC_LIBFT := libft/lib/libft.a

CFLAGS := -Wall -Wextra -Werror

FILES := main.c \
	cmd_funcs.c \
	exec_funcs_utils.c \
	exec_funcs.c \
	fildes_funcs.c \
	misc.c \
	pipe_funcs.c
SRC := $(addprefix $(SRC_DIR)/, $(FILES))
OBJ := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC))

FILES_BONUS := main_bonus.c \
	cmd_funcs.c \
	exec_funcs_utils.c \
	exec_funcs.c \
	fildes_funcs.c \
	misc.c \
	pipe_funcs.c
SRC_BONUS := $(addprefix $(SRC_DIR)/, $(FILES_BONUS))
OBJ_BONUS := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_BONUS))

.SILENT:

all: $(NAME)

rmbonus:
	rm -f bonus

$(NAME): $(STATIC_LIBFT) $(OBJ) rmbonus
	$(CC) $(OBJ) -o $@ $(CFLAGS) -I $(INC_DIR) -I $(LIBFT_INC_DIR) $(STATIC_LIBFT)
	echo "pipex done"

$(STATIC_LIBFT):
	$(MAKE) -C libft

$(BIN_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) -I $(LIBFT_INC_DIR)

clean:
	rm -f ./bin/*.o
	$(MAKE) -C libft clean

fclean: clean rmbonus
	rm -f $(NAME)
	$(MAKE) -C libft fclean

re: fclean all

bonus: $(STATIC_LIBFT) $(OBJ_BONUS)
	@touch bonus
	$(CC) $(OBJ_BONUS) -o $(NAME) $(CFLAGS) -I $(INC_DIR) -I $(LIBFT_INC_DIR) $(STATIC_LIBFT)
	@echo "pipex bonus done"

.PHONY: all, clean, fclean, re, rmbonus

