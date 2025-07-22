SRCS	=	main.c \
			tokenization.c \
			utils.c \
			builtins/cd_builtin.c \
			builtins/echo_builtin.c \
			builtins/pwd_builtin.c \
			builtins/env_utils.c \
			tokenization_utils.c\
			free.c

OBJS	= $(SRCS:.c=.o)

LIBFT_DIR	=	libft
LIBFT	=	$(LIBFT_DIR)/libft.a


CC		= cc

CFLAGS	= -Wall -Wextra -Werror

LDFLAGS = -lreadline

NAME	= minishell

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

clean:
	@echo "[DELETING...]"
	rm -rf $(OBJS)
	@make -C $(LIBFT_DIR) clean
	@echo "[DELETED]"

fclean: clean
	rm -rf $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all fclean clean re