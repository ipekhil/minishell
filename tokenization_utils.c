/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:09 by sude              #+#    #+#             */
/*   Updated: 2025/07/22 00:26:50 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_quoted_token(char *line, int *i)
{
	char	quote;
	char	*token;
	int		len;
	int		start;

	len = 0;
	quote = line[(*i)++];
	start = *i;
	while (line[*i] != quote && line[*i] != '\0')
	{
		len++;
		(*i)++;
	}
	if (line[*i] == '\0')
	{
		printf("Error: Unmatched quote\n");
		return (NULL);
	}
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	token[len] = '\0';
	ft_strlcpy(token, &line[start], len + 1);
	(*i)++;
	return (token);
}

void	add_token(t_token **tokens, char *token, int type)
{
	t_token	*new_token;
	t_token	*tmp;

	tmp = NULL;
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->value = strdup(token);
	new_token->type = type;
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
	while ((!ft_isspace(line[*i]) && line[*i] != '"'
			&& line[*i] != '\'') && line[*i] != '\0')
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
