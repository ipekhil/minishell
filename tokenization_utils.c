/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:21:09 by sude              #+#    #+#             */
/*   Updated: 2025/08/16 17:26:43 by sude             ###   ########.fr       */
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

void	init_tvar(t_var *var)
{
	var->has_quotes = 0;
	var->has_single_quotes = 0;
	var->has_dollar = 0;
}

void	determine_token_type(t_var *var, char *line, int i)
{
	if (line[i] == '"')
	{
		var->has_quotes = 1;
		while (line[++i] && line[i] != '"')
		{
			if (line[i] == '$')
				var->has_dollar = 1;
		}
		i++;
	}
	else if (line[i] == '\'')
	{
		var->has_single_quotes = 1;
		while (line[++i] && line[i] != '\'')
			;
		i++;
	}
	else
	{
		while (line[i] && !ft_isspace(line[i]) && !ft_isoperator(line[i])
			&& line[i] != '"' && line[i] != '\'')
			if (line[i++] == '$')
				var->has_dollar = 1;
	}
}

int	ret_token_type(t_var *var, char *line, int i)
{
	init_tvar(var);
	determine_token_type(var, line, i);
	if (var->has_quotes && var->has_dollar)
		return (DOUBLE_QUOTED_EXPANDABLE);
	else if (var->has_quotes)
		return (DOUBLE_QUOTED);
	else if (var->has_single_quotes)
		return (SINGLE_QUOTED);
	else if (var->has_dollar)
		return (EXPANDABLE_WORD);
	else
		return (WORD);
}
