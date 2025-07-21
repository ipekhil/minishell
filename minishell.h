#ifndef MINISHELL_H
# define MINISHELL_H


#define PIPE         0 // |
#define RDR_IN       1 // <
#define RDR_OUT      2 // >
#define RDR_APPEND   3 // >>
#define HEREDOC      4 // <<
#define WORD  		 5

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token {
    char          *value;
    int				type;
    struct s_token *next;
} t_token;

typedef struct s_data
{
	char	*line;
    t_token *tokens;
}			t_data;



void routine_loop(t_data *data);
void tokenization(t_data *data);

#endif