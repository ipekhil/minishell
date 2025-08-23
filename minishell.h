/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:05 by sude              #+#    #+#             */
/*   Updated: 2025/08/23 22:29:38 by sude             ###   ########.fr       */
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
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

extern volatile sig_atomic_t	g_signal_flag;

typedef enum e_token_type
{
	HEREDOC = 0,
	APPEND = 1,
	INPUT = 2,
	OUTPUT = 3,
	PIPE = 4,
	WORD = 5,
	DOUBLE_QUOTED = 6,
	SINGLE_QUOTED = 7,
	EXPANDABLE_WORD = 8,
	DOUBLE_QUOTED_EXPANDABLE = 9
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
	int						hdoc_int;
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

//apply_redir.c
void	apply_redirections(t_data *data, t_redirection *redir);

//conc_utils.c
void	handle_merge(t_exp **nodes, t_exp *start, t_exp *end);

//concatenator.c
void	concatenator(t_data *data);

//BUILTINS
//cd_builtin.c
void	add_or_update_env(t_env **env, char *key, char *value);
int		cd_builtin(t_data *data, char **argv);
//echo_builtin.c
int		echo_builtin(char **input);
//env_builtin.c
char	**copy_env(char **envp);
int		env_builtin(t_data *data, char **args);
//exit_builtin.c
void	exit_builtin(t_data *data, char **args);
void	write_err(const char *msg, const char *arg, const char *msg2);
//export_builtin.c
int		export_builtin(t_data *data, char **args);
t_env	**is_there_var_in_env(t_env **env, char *key, char *value);
//pwd_builtin.c
int		pwd_builtin(void);
//unset_builtin.c
int		unset_builtin(t_data *data, char **args);
int		is_valid_key(char *key);
//exec_child.c
void	child_process(t_data *data, t_parser *cmd, int *pipe_fds, int prv_pipe);
//exec_child2.c
char	*find_command_path(t_data *data, char *cmd, char **env);
//exec_control.c
void	pre_file_check(t_data *data, char *cmd, int *exit);
void	handle_fork_error(t_parser *cmds, int *pipe_fds, int *prev_read_fd);
void	handle_err_and_exit(t_data *data, char *cmd, const char *msg, int code);

//exec_utils.c
int		heredoc_was_interrupted(t_parser *cmds);
int		is_builtin(char *cmd);
void	execute_builtin(t_data *data, char **args);
int		lstsize(t_env *env);
void	setup_child_signals(void);

//executor.c
void	executor(t_data *data);

//exp_dollar.c
void	get_len(t_data *data, char *first_val, int i, int *len);
void	expand_token_value(t_data *data, char *first_val, char *new_val, int i);

//exp_utils.c
char	*extract_key(char *token_val);
char	*get_value_of_key(t_data *data, char *key, int *app_fl);
void	append_value(char *exp_value, char *value, int *a_index);

//expander.c
void	expander(t_data *data);

//free.c
void	free_all(t_data *data, int free_env_flag);
void	free_array(char **arr);
//free2.c
void	free_token(t_token *head);
void	free_expander(t_exp *head);
void	free_parser(t_parser *head);

//get_env.c
void	ft_lstadd_back(t_env **head, t_env *new_node);
void	get_env(t_env **env, char **envp);

//heredoc.c
void	apply_heredoc(t_data *data);

//parse_syntax_control.c
int		syntax_control(t_exp *tokens);

//parser.c
void	parser(t_data *data);

//redirection.c
int		count_args(t_exp *start, t_exp *end);
int		parse_command(t_exp *start, t_exp *end, t_parser *node);

//signal.c
void	setup_signal_main(void);

void	setup_signal_main(void);
void	setup_signal_heredoc(void);
void	signal_dfl(void);

//token_operator.c
void	add_token(t_token **tokens, char *token, int type, int concat_flag);
void	handle_operator_token(t_token **tokens, char *line, int *i);

//tokenization_utils.c
int		check_unmatched_quotes(char *line);
char	*get_token(char *line, int *i);
void	init_tvar(t_var *var);
void	determine_token_type(t_var *var, char *line, int i);
int		ret_token_type(t_var *var, char *line, int i);

//tokenization.c
int		tokenization(t_data *data);

//update_char_env.c
char	**convert_env_to_array(t_env *env);

//utils.c
int		ft_isspace(char c);
int		ft_isoperator(char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strcat(char *dest, const char *src);
#endif