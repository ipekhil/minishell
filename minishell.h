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
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct s_token {
    char          *value;
    int				type;
    struct s_token *next;
} t_token;

typedef struct s_data
{
	char	*line;
    t_token *tokens;
	int		token_count;
	char	**env;
}			t_data;



void routine_loop(t_data *data);
void tokenization(t_data *data);


int		is_builtin(char *cmd);
void	execute_builtin(t_data	*data);
void	cd_builtin(char **argv, char ***envp);
int		echo_builtin(char **input);
void	pwd_builtin(void);
void	update_env_var(char ***envp, const char *name, const char *value);
char	**copy_env(char **envp);


int	ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *dest, const char *src);
#endif