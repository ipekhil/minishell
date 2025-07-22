/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:05 by sude              #+#    #+#             */
/*   Updated: 2025/07/22 14:07:46 by sude             ###   ########.fr       */
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
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

typedef struct s_token
{
	char			*value;
	int				type;
	struct s_token	*next;
}					t_token;

typedef struct s_data
{
	char	*line;
    t_token *tokens;
	int		token_count;
	char	**env;
}			t_data;


void	routine_loop(t_data *data);
int		tokenization(t_data *data);
t_token	*split_tokens(char **space_sep);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*get_quoted_token(char *line, int *i);
char	*get_token(char *line, int *i);
void	add_token(t_token **tokens, char *token, int type);
int		ft_isspace(char c);
int		ft_isoperator(char c);
void	free_token(t_token *head);


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