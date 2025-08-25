/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: staylan <staylan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:25:09 by staylan           #+#    #+#             */
/*   Updated: 2025/08/25 20:23:56 by staylan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	get_operator_type(char *line, int i)
{
	if (line[i] == '<' && line[i + 1] == '<')
		return (0);
	else if (line[i] == '>' && line[i + 1] == '>')
		return (1);
	else if (line[i] == '<')
		return (2);
	else if (line[i] == '>')
		return (3);
	else if (line[i] == '|')
		return (4);
	return (-1);
}

static char	*get_op_token(char *line, int size)
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
	add_token(tokens, token, type, 0);
	if (token)
		free(token);
	*i += size;
}
