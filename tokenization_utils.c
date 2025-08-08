/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:09 by sude              #+#    #+#             */
/*   Updated: 2025/08/08 15:28:52 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_unmatched_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '"' || line[i] == '\'')
		{
			quote = line[i++];
			while (line[i] && line[i] != quote)
				i++;
			if (!line[i])
			{
				printf("minishell: syntax error: unmatched quote\n");
				return (0);
			}
			i++;
		}
		else
			i++;
	}
	return (1);
}

void	add_token(t_token **tokens, char *token, int type, int concat_flag)
{
	t_token	*new_token;
	t_token	*tmp;

	tmp = NULL;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = ft_strdup(token);
	new_token->type = type;
	new_token->concat_w_next = concat_flag;
	new_token->next = NULL;
	if (!*tokens)
		*tokens = new_token;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

char	*get_token(char *line, int *i)
{
	char	*token;
	int		len;
	int		start;

	len = 0;
	start = *i;
	while ((!ft_isspace(line[*i]) && line[*i] != '\0' && line[*i] != '"'
			&& line[*i] != '\'') && line[*i + 1] != '\0')
	{
		if (ft_isoperator(line[*i]))
			break ;
		len++;
		(*i)++;
	}
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	token[len] = '\0';
	ft_strlcpy(token, &line[start], len + 1);
	return (token);
}
