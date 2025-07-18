SRCS	=	main.c

OBJS	= $(SRCS:.c=.o)

CC		= cc

CFLAGS	= -Wall -Wextra -Werror

LDFLAGS = -lreadline

NAME	= minishell

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)

clean:
	@echo "[DELETING...]"
	rm -rf $(OBJS)
	@echo "[DELETED]"

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re