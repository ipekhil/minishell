/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:25:07 by sude              #+#    #+#             */
/*   Updated: 2025/07/23 23:16:14 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_operator_type(char *line, int i)
{
	if (line[i] == '<' && line[i + 1] == '<')
		return (0);
	else if (line[i] == '>' && line[i + 1] == '>')
		return (1);
	else if (line[i] == '<' )
		return (2);
	else if (line[i] == '>')
		return (3);
	else if (line[i] == '|')
		return (4);
	return (-1);
}

char	*get_op_token(char *line, int size)
{
	char	*token;

	token = malloc(sizeof(char) * (size + 1));
	if (!token)
		return (NULL);
	ft_strlcpy(token, line, size + 1);
	return (token);
}

void	handle_operator_token(t_token **tokens, char *line, int *i)
{
	int		type;
	int		size;
	char	*token;

	type = get_operator_type(&line[*i], 0);
	size = 1;
	if (type == 0 || type == 1)
		size = 2;
	token = get_op_token(&line[*i], size);
	add_token(tokens, token, type);
	*i += size;
}

static int	handle_token(t_token **tokens, char *line, int *i, int quoted)
{
	char	*token;

	token = NULL;
	if (quoted)
		token = get_quoted_token(line, i);
	else if (quoted == 0)
		token = get_token(line, i);
	if (token == NULL)
		return (-1);
	add_token(tokens, token, WORD);
	return (0);
}

int	tokenization(t_data *data)
{
	int			i;

	i = 0;
	while (data->line[i])
	{
		if (data->line[i] == '"' || data->line[i] == '\'')
		{
			if (handle_token(&(data->tokens), data->line, &i, 1) == -1)
				return (-1);
		}
		else if (ft_isspace(data->line[i]))
			while (ft_isspace(data->line[i]))
				i++;
		else if (ft_isoperator(data->line[i]))
			handle_operator_token(&(data->tokens), data->line, &i);
		else
		{
			if (handle_token(&(data->tokens), data->line, &i, 0) == -1)
				return (-1);
		}
	}
	expander(data);
	return (0);
}
