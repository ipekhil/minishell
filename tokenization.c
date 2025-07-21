/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:25:07 by sude              #+#    #+#             */
/*   Updated: 2025/07/22 00:27:24 by sude             ###   ########.fr       */
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
	token[size] = '\0';
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
	free(token);
	*i += size;
}

void	handle_quoted_token(t_token **tokens, char *line, int *i)
{
	char	*token;

	token = get_quoted_token(line, i);
	add_token(tokens, token, WORD);
	free(token);
}

void	tokenization(t_data *data)
{
	t_token		*tokens;
	char		*token;
	int			i;

	i = 0;
	tokens = NULL;
	while (data->line[i])
	{
		while (ft_isspace(data->line[i]))
			i++;
		if (data->line[i] == '"' || data->line[i] == '\'')
			handle_quoted_token(&tokens, data->line, &i);
		else if (ft_isoperator(data->line[i]))
			handle_operator_token(&tokens, data->line, &i);
		else
		{
			token = get_token(data->line, &i);
			add_token(&tokens, token, WORD);
			free(token);
		}
	}
	while(tokens)
	{
		printf("%s %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}
