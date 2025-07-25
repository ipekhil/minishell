/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:05 by sude              #+#    #+#             */
/*   Updated: 2025/07/25 23:55:58 by sude             ###   ########.fr       */
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

typedef struct s_data
{
	char		*line;
	t_token		*tokens;
	t_env		*env;
	t_expander	*expander;
}				t_data;

typedef struct s_parser
{
	char 		**args;
	struct s_parser	*next;
}		t_parser;

void	free_expander(t_expander *head);
void	routine_loop(t_data *data);
int		tokenization(t_data *data);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
int		ft_strcmp(char *s1, char *s2);
int		ft_strlen(const char *s);
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
t_parser *parser(t_data *data);
void print_parser(t_parser *parser);




#endif