SRCS = main.c \
	PARSER/tokenization.c \
	PARSER/tokenization_utils.c\
	PARSER/token_operator.c\
	PARSER/expander.c \
	PARSER/exp_utils.c \
	PARSER/exp_dollar.c \
	PARSER/parser.c \
	PARSER/parse_syntax_control.c \
	PARSER/concatenator.c \
	PARSER/conc_utils.c \
	UTILS/utils.c \
	UTILS/free.c \
	UTILS/free2.c \
	UTILS/signals.c \
	ENV/update_char_env.c \
	ENV/get_env.c \
	EXEC/redirection.c \
	EXEC/executor.c \
	EXEC/apply_redir.c \
	EXEC/exec_control.c \
	EXEC/exec_child.c \
	EXEC/exec_child2.c \
	EXEC/exec_utils.c \
	EXEC/heredoc.c \
	BUILTINS/export_builtin.c \
	BUILTINS/unset_builtin.c \
	BUILTINS/cd_builtin.c \
	BUILTINS/echo_builtin.c \
	BUILTINS/pwd_builtin.c \
	BUILTINS/env_builtin.c \
	BUILTINS/exit_builtin.c 

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -g -Wall -Wextra -Werror -I./INCLUDE
LDFLAGS = -lreadline
NAME = minishell

LIBFT_DIR = INCLUDE/libft
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