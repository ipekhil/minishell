SRCS	=	main.c \
			tokenization.c \
			utils.c \
			tokenization_utils.c\
			free.c \
			get_env.c \
			expander.c \
			parser.c \
			redirection.c \
			parse_syntax_control.c \
			signals.c \
			heredoc.c

OBJS	= $(SRCS:.c=.o)

CC		= cc

CFLAGS	= -g -Wall -Wextra -Werror

LDFLAGS = -lreadline

NAME	= minishell

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean:
	@echo "[DELETING...]"
	rm -rf $(OBJS)
	@echo "[DELETED]"

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re