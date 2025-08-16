SRCS = main.c \
	tokenization.c \
	utils.c \
	tokenization_utils.c\
	token_operator.c\
	free.c \
	get_env.c \
	expander.c \
	exp_utils.c \
	parser.c \
	redirection.c \
	parse_syntax_control.c \
	cd_builtin.c \
	echo_builtin.c \
	pwd_builtin.c \
	env_utils.c \
	executor.c \
	apply_redir.c \
	exec_control.c \
	exec_child_process.c \
	exec_utils.c \
	update_char_env.c \
	export_builtin.c \
	env_builtin.c \
	unset_builtin.c \
	heredoc.c \
	signals.c \
	exit_builtin.c \
	concatenator.c \
	conc_utils.c

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