SRCS = main.c \
	tokenization.c \
	utils.c \
	tokenization_utils.c\
	free.c \
	get_env.c \
	expander.c \
	parser.c \
	redirection.c \
	parse_syntax_control.c \
	cd_builtin.c \
	echo_builtin.c \
	pwd_builtin.c \
	env_utils.c \
	executor.c \
	export_builtin.c \
	env_builtin.c \
	unset_builtin.c \
	heredoc.c \
	signals.c \
	exit_builtin.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -g -Wall -Wextra -Werror
LDFLAGS = -lreadline
NAME = minishell

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

clean:
	@echo "[DELETING...]"
	@rm -rf $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@echo "[DELETED]"

fclean: clean
	@rm -rf $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all fclean clean re