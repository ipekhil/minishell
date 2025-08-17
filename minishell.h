/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:05 by sude              #+#    #+#             */
/*   Updated: 2025/08/17 21:38:20 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <unistd.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
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
	int				concat_w_next;
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

typedef struct s_exp
{
	char				*exp_value;
	int					type;
	int					concat_w_next;
	struct s_exp		*next;
}				t_exp;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	char					*heredoc_content;
	int 					hdoc_int;
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
	t_exp		*expander;
	t_parser	*parser;
	t_var		var;
	int			last_exit_status;
	int			should_exit;
}		t_data;

void	routine_loop(t_data *data);

//tokenization
int		tokenization(t_data *data);
int		check_unmatched_quotes(char *line);
char	*get_token(char *line, int *i);
void	add_token(t_token **tokens, char *token, int type, int concat_flag);
void	handle_operator_token(t_token **tokens, char *line, int *i);
int		ret_token_type(t_var *var, char *line, int i);

//utils.c
int		ft_isspace(char c);
int		ft_isoperator(char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strcat(char *dest, const char *src);

void	get_env(t_env **env, char **envp);
void	apply_heredoc(t_data *data);


// Concatenator
void	handle_merge(t_exp **nodes, t_exp *start, t_exp *end);
void	concatenator(t_data *data);

//expander
void	expand_token_value(t_data *data, char *first_val, char *new_val, int i);
void	expander(t_data *data);
char	*get_value_of_key(t_data *data, char *key, int *app_fl);
char	*extract_key(char *token_val);
void	get_len(t_data *data, char *first_val, int i, int *len);

// Parser
void	parser(t_data *data);
int		syntax_control(t_exp *tokens);

//parser_utils
int	count_args(t_exp *start, t_exp *end);
int	parse_command(t_exp *start, t_exp *end, t_parser *node);


// Builtins
int		cd_builtin(t_data *data, char **argv);
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
char	*find_command_path(char *cmd, char **env);
char	*create_full_path(char *dir, char *cmd);
//apply_redir
void	apply_redirections(t_data *data, t_redirection *redir);
// exec_control
void	handle_err_and_exit(t_data *data, char *cmd, const char *msg, int code);
void	handle_fork_error(t_parser *cmds, int *pipe_fds, int *prev_read_fd);
void	pre_file_check(t_data *data, char *cmd, int *exit);
//exec_child_process
void	child_process(t_data *data, t_parser *cmd, int *pipe_fds, int prev_pipe);
//update_char_env
char	**convert_env_to_array(t_env *env);
//exec_utils
void	setup_child_signals(void);
int		lstsize(t_env *env);

// Environment
char	**copy_env(char **envp);
void	ft_lstadd_back(t_env **head, t_env *new_node);


// Print functions
void	ser(t_parser *parser);
void	print_redirections(t_redirection *redir);

//free
void	free_array(char **arr);
void	free_parser(t_parser *head);
void	free_expander(t_exp *head);
void	free_all(t_data *data);
void	free_token(t_token *head);
void	free_env(t_env *head);
void	free_redirection(t_redirection *head);
void	free_merged(t_exp *start);

//exporttan cd için
void	add_or_update_env(t_env **env, char *key, char *value);
t_env	**is_there_var_in_env(t_env **env, char *key, char *value);

//redirection
void	add_redir_to_parser(t_redirection **list, t_redirection *new_node);
t_redirection	*new_redirection_node(t_exp *current);

#endif