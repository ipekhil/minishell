/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:05 by sude              #+#    #+#             */
/*   Updated: 2025/08/05 05:23:07 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

extern	int	g_signal_exit;

typedef enum e_token_type
{
	WORD = 5,
	DOUBLE_QUOTED = 6,
	SINGLE_QUOTED = 7,
	EXPANDABLE_WORD = 8,
	DOUBLE_QUOTED_EXPANDABLE = 9,

	HEREDOC = 0, // <<
	APPEND = 1, // >>
	INPUT = 2, // <
	OUTPUT = 3, // >
	PIPE = 4 // |
}		t_token_type;

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}	t_token;

typedef struct s_var
{
	int	has_quotes;
	int	has_single_quotes;
	int	has_dollar;
}			t_var;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_expander
{
	char				*exp_value;
	int					type;
	struct s_expander	*next;
}				t_expander;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	char					*heredoc_content;
	char					*delimiter;
	struct s_redirection	*next;
}		t_redirection;

typedef struct s_parser
{
	char			**args;
	t_redirection	*redirection;
	int				is_error;
	struct s_parser	*next;
}		t_parser;

typedef struct s_data
{
	char		**char_env;
	char		*line;
	t_token		*tokens;
	t_env		*env;
	t_expander	*expander;
	t_parser	*parser;
	t_var		var;
	int			last_exit_status;
	int			should_exit;
}		t_data;

int		ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *dest, const char *src);
void	routine_loop(t_data *data);
int		tokenization(t_data *data);
int		check_unmatched_quotes(char *line);
char	*get_token(char *line, int *i);
void	add_token(t_token **tokens, char *token, int type);
int		ft_isspace(char c);
int		ft_isoperator(char c);
void	get_env(t_env **env, char **envp);
void	expander(t_data *data);

// Parser
void	parser(t_data *data);
int		syntax_control(t_expander *tokens);
int		parse_command(t_expander *start, t_expander *end, t_parser *node);

// Builtins
int		cd_builtin(char **argv, char ***envp);
int		echo_builtin(char **input);
int		pwd_builtin(void);
void	update_env_var(char ***envp, const char *name, const char *value);
int		is_builtin(char *cmd);
void	execute_builtin(t_data *data, char **args);
int		export_builtin(t_data *data, char **args);
int		is_valid_key(char *key);
int		env_builtin(t_data *data, char **args);
int		unset_builtin(t_data *data, char **args);
void	exit_builtin(t_data *data, char **args);

//signals
void	signal_handlers_main(void);

// Executor functions
void	executor(t_data *data);
void	execute_external(t_data *data, char **args);
char	*find_command_path(char *cmd, char **env);
char	*create_full_path(char *dir, char *cmd);

// Environment
char	**copy_env(char **envp);
void	ft_lstadd_back(t_env **head, t_env *new_node);


// Print functions
void	ser(t_parser *parser);
void	print_redirections(t_redirection *redir);

//free
void	free_array(char **arr);
void	free_parser(t_parser *head);
void	free_expander(t_expander *head);
void	free_all(t_data *data);
void	free_token(t_token *head);

#endif