/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 00:25:07 by sude              #+#    #+#             */
/*   Updated: 2025/08/22 19:51:47 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	conc_flag_update(int *conc_flag, int *len, char *line, int temp_i)
{
	while (line[temp_i] && !ft_isspace(line[temp_i])
		&& !ft_isoperator(line[temp_i]) && line[temp_i] != '"'
		&& line[temp_i] != '\'')
	{
		temp_i++;
		(*len)++;
	}
	if (!ft_isspace(line[temp_i]) && !ft_isoperator(line[temp_i])
		&& line[temp_i] != '\0')
		*conc_flag = 1;
}

static void	get_conc_flag(char *line, int temp_i, int *len, int *conc_flag)
{
	char	quote;

	quote = line[temp_i];
	if (quote == '"' || quote == '\'')
	{
		while (line[++temp_i] && line[temp_i] != quote)
			(*len)++;
		if (line[temp_i] == quote)
			temp_i++;
		if (!ft_isspace(line[temp_i]) && !ft_isoperator(line[temp_i])
			&& line[temp_i] != '\0')
			*conc_flag = 1;
	}
	else
		conc_flag_update(conc_flag, len, line, temp_i);
}

static char	*extract_quoted_text(char *result, char *line, int *i, int result_i)
{
	if (line[*i] == '"')
	{
		while (line[++(*i)] && line[*i] != '"')
		{
			result[result_i++] = line[*i];
		}
		(*i)++;
	}
	else if (line[*i] == '\'')
	{
		while (line[++(*i)] && line[*i] != '\'')
		{
			result[result_i++] = line[*i];
		}
		(*i)++;
	}
	else
	{
		while (line[*i] && !ft_isspace(line[*i]) && !ft_isoperator(line[*i])
			&& line[*i] != '"' && line[*i] != '\'')
			result[result_i++] = line[(*i)++];
	}
	result[result_i] = '\0';
	return (result);
}

static void	get_combined_token(t_data *data, int *i)
{
	int		len;
	int		temp_i;
	int		conc_flag;
	int		token_type;
	char	*token;

	temp_i = *i;
	len = 0;
	conc_flag = 0;
	get_conc_flag(data->line, temp_i, &len, &conc_flag);
	token = malloc(sizeof(char) * (len + 1));
	if (!token)
		return ;
	extract_quoted_text(token, data->line, i, 0);
	if (!token)
	{
		free_all(data, 1);
		return ;
	}
	token_type = ret_token_type(&data->var, data->line, temp_i);
	add_token(&(data->tokens), token, token_type, conc_flag);
	free(token);
}

int	tokenization(t_data *data)
{
	int	i;

	i = 0;
	if (!check_unmatched_quotes(data->line))
		return (-1);
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
