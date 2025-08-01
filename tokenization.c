/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:25:07 by sude              #+#    #+#             */
/*   Updated: 2025/08/02 18:32:20 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_operator_type(char *line, int i)
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
	if (token)
		free(token);
	*i += size;
}

void	get_combined_len(char *line, int temp_i, int *len)
{
	while (line[temp_i] && !ft_isspace(line[temp_i]) && !ft_isoperator(line[temp_i]))
	{
		if (line[temp_i] == '"')
		{
			while (line[++temp_i] && line[temp_i] != '"')
				(*len)++;
			if (line[temp_i] == '"')
				temp_i++;
		}
		else if (line[temp_i] == '\'')
		{
			while (line[++temp_i] && line[temp_i] != '\'')
				(*len)++;
			if (line[temp_i] == '\'')
				temp_i++;
		}
		else
		{
			temp_i++;
			(*len)++;
		}
	}
}

char	*extract_quoted_text(char *result, char *line, int *i, int result_i)
{
	while (line[*i] && !ft_isspace(line[*i]) && !ft_isoperator(line[*i]))
	{
		if (line[*i] == '"')
		{
			while (line[++(*i)] && line[*i] != '"')
			{
				result[result_i++] = line[*i];
			}
			(*i)++;//SONDAKİ TIRNAĞI GEÇ
		}
		else if (line[*i] == '\'')
		{
			while (line[++(*i)] && line[*i] != '\'')
			{
				result[result_i++] = line[*i];
			}
			(*i)++;//SONDAKİ TIRNAĞI GEÇ
		}
		else
			result[result_i++] = line[(*i)++];
	}
	result[result_i] = '\0';
	return (result);
}

void	init_tvar(t_var *var)
{
	var->has_quotes = 0;
	var->has_single_quotes = 0;
	var->has_dollar = 0;
}

void	determine_token_type(t_var *var, char *line, int i)
{
	while (line[i] && !ft_isspace(line[i]) && !ft_isoperator(line[i]))
	{
		if (line[i] == '"')
		{
			var->has_quotes = 1;
			while (line[++i] && line[i] != '"')
			{
				if (line[i] == '$')
					var->has_dollar = 1;
			}
			i++;//son trınak geç
		}
		else if (line[i] == '\'')
		{
			var->has_single_quotes = 1;
			while (line[++i] && line[i] != '\'')
				;
			i++;//son trınak geç
		}
		else
		{
			if (line[i++] == '$')
				var->has_dollar = 1;
		}
	}
}

int ret_token_type(t_var *var, char *line, int i)
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

void	get_combined_token(t_data *data, int *i)
{
    int		len;
	int		temp_i;
	int		result_i;
    char	*token;
	int		token_type;

	temp_i = *i;
	result_i = 0;
	len = 0;
	get_combined_len(data->line, temp_i, &len);
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return ;
	extract_quoted_text(token, data->line, i, result_i);
	if (!token)
	{
		free_all(data);
			return ;
	}
	token_type = ret_token_type(&data->var, data->line, temp_i);
	add_token(&(data->tokens), token, token_type);
	free(token);
}

int tokenization(t_data *data)
{
	int i = 0;

    if (!check_unmatched_quotes(data->line)){
        return (-1); 
	}
	while (data->line[i])
	{
		if (ft_isspace(data->line[i]))
		{
			while (ft_isspace(data->line[i]))
				i++;
		}
		else if (ft_isoperator(data->line[i]))
		{
			handle_operator_token(&(data->tokens), data->line, &i);
		}
		else
			get_combined_token(data, &i);
	}
	expander(data);
	return (0);
}
