/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_syntax_control.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:39:13 by hilalipek         #+#    #+#             */
/*   Updated: 2025/08/08 21:01:04 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_syntax_error(t_exp *tokens)
{
	if (tokens)
		printf("-minishell: syntax error near unexpected token `%s'\n"\
			, tokens->exp_value);
	else
		printf("-minishell: syntax error near unexpected token `newline'\n");
}

static int	heredoc_syntax_control(t_exp *token)
{
	if (!token->next)
	{
		print_syntax_error(NULL);
		return (0);
	}
	if (token->next->type != 5)
	{
		print_syntax_error(token->next);
		return (0);
	}
	token = token->next;
	return (1);
}

static int pipe_syntax_control(t_exp *tokens, t_exp *current)
{
	// başta | olması durumu
	if (tokens == current && current && current->type == 4)
	{
		print_syntax_error(current);
		return (0);
	}
	// || olması durumu
	if (current && current->type == 4 &&
		current->next && current->next->type == 4)
	{
		print_syntax_error(current->next);
		return (0);
	}
	if (current && current->type == 4 && current->next
		&& (current->next->type == 0 || current->next->type == 1
			|| current->next->type == 2 || current->next->type == 3))
	{
		print_syntax_error(current->next);
		return (0);
	}
	// sonda | olması durumu
	if (current && current->type == 4 && !current->next)
	{
		print_syntax_error(NULL);
		return (0);
	}
	return (1);
}

static int redirection_syntax_control(t_exp *current)
{
	if (!current->next)
	{
		print_syntax_error(NULL);
		return (0);
	}
	if (current->next->type != 5)
	{
		print_syntax_error(current->next);
		return (0);
	}
	return (1);
}

int syntax_control(t_exp *tokens)
{
	t_exp *current;

	current = tokens;
	if (!current)
		return (1);
	if (!pipe_syntax_control(tokens, current))
		return (0);
	while (current)
	{
		if (current->type == 1 || current->type == 2 || current->type == 3)
		{
			if (!redirection_syntax_control(current))
				return (0);
		}
		else if (current->type == 0)
		{
			if (!heredoc_syntax_control(current))
				return (0);
		}
		else if (current->type == 4)
		{
			if (!pipe_syntax_control(tokens, current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}