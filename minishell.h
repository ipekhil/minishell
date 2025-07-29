/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hilalipek <hilalipek@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:05 by sude              #+#    #+#             */
/*   Updated: 2025/07/29 20:35:42 by hilalipek        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define HEREDOC      0 // <<
# define RDR_APPEND   1 // >>
# define RDR_IN       2 // <
# define RDR_OUT      3 // >
# define PIPE         4 // |
# define WORD  		  5

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_expander
{
	char				*exp_value;
	int					type;
	struct s_expander	*next;
}						t_expander;

typedef struct s_redirection
{
	int						type;
	char					*filename;
	struct s_redirection	*next;
}		t_redirection;

typedef struct s_parser
{
	char 			**args;
	t_redirection	*redirection;
	int				is_error;
	struct s_parser	*next;
}		t_parser;

typedef struct s_data
{
	char		*line;
	t_token		*tokens;
	t_env		*env;
	t_expander	*expander;
	t_parser	*parser;
}				t_data;

void	free_parser(t_parser *head);
void	free_expander(t_expander *head);
void	routine_loop(t_data *data);
int		tokenization(t_data *data);
char	*get_quoted_token(char *line, int *i);
char	*get_token(char *line, int *i);
void	add_token(t_token **tokens, char *token, int type);
int		ft_isspace(char c);
int		ft_isoperator(char c);
void	free_token(t_token *head);
void	get_env(t_env **env, char **envp);
void	expander(t_data *data);
void	free_all(t_data *data);
char	*ft_strdup(const char *s1);

//parser
void parser(t_data *data);
//parse_syntax
int syntax_control(t_expander *tokens);
//redirection
int	parse_command(t_expander *start, t_expander *end, t_parser *node);


//libft
int		ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
int		ft_strcmp(char *s1, char *s2);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);



//print
void print_parser(t_parser *parser);
void print_redirections(t_redirection *redir);
#endif