/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:39:20 by hilalipek         #+#    #+#             */
/*   Updated: 2025/08/16 23:33:05 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_exp *start, t_exp *end)
{
	t_exp *current;
	int	count;

	count = 0;
	current = start;
	while (current != end)
	{
		if (current->type == 5)
			count++;
		current = current->next;
	}
	return (count);
}

static t_parser *new_parser_node(t_exp *start, t_exp *end)
{
	t_parser	*node;
	int			count;

	node = NULL;
	node = malloc(sizeof(t_parser));
	if (!node)
		return (NULL);
	node->args = NULL;
	node->redirection = NULL;
	node->is_error = 0;
	count = count_args(start, end);
	node->args = ft_calloc((count + 1), sizeof(char *));
	if (!node->args)
		return (NULL);
	if (!parse_command(start, end, node))
	{
		node->is_error = 1;
		printf("HATA\n");
		return (NULL);
	}
	node->args[count] = NULL;
	node->next = NULL;
	return (node);
}

static void add_command_to_parser(t_parser **list, t_parser *new_node)
{
	t_parser *current;

	if (!list || !new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

static void	free_parser_on_error(t_data *data)
{
	if (data->parser)
	{
		free_parser(data->parser);
		data->parser = NULL;
	}
}

void all_tokens_parse(t_data *data, t_exp *tokens)
{
	t_exp *current;
	t_exp *start;
	t_parser *new_node;

	data->parser = NULL;
	current = tokens;
	start = current;
	while (current)
	{
		if (current->type == 4)
		{
			new_node = new_parser_node(start, current);
			if (!new_node)
			{
				free_parser_on_error(data);
				return ;
			}
			add_command_to_parser(&data->parser, new_node);
			start = current->next;
		}
		current = current->next;
	}
	new_node = new_parser_node(start, current);
	if (!new_node)
	{
		free_parser_on_error(data);
		return ;
	}
	add_command_to_parser(&data->parser, new_node);
}

void print_parser(t_parser *parser)
{
    int cmd_count = 0;
    
    // Parser listesi üzerinde döngüye başla
    while (parser)
    {
        printf("--------------------------------------\n");
        printf("Komut #%d:\n", ++cmd_count);
        
        // Komutun argümanlarını yazdır
        printf("  Args: ");
        if (parser->args)
        {
            int i = 0;
            while (parser->args[i])
            {
                printf("'%s'", parser->args[i]);
                if (parser->args[i + 1])
                    printf(", ");
                i++;
            }
        }
        else
        {
            printf("(none)");
        }
        printf("\n");
        
        // Yönlendirmeleri yazdır
        printf("  Redirections:\n");
        if (parser->redirection)
        {
            t_redirection *redir = parser->redirection;
            while (redir)
            {
                printf("    - Type: ");
                if (redir->type == INPUT)
                    printf("INPUT (<)\n");
                else if (redir->type == OUTPUT)
                    printf("OUTPUT (>)\n");
                else if (redir->type == APPEND)
                    printf("APPEND (>>)\n");
                else if (redir->type == HEREDOC)
                    printf("HEREDOC (<<)\n");
                
                printf("      Filename: '%s'\n", redir->filename);
				printf("      Heredoc delimiter: '%s'\n", redir->delimiter);
                redir = redir->next;
            }
        }
        else
        {
            printf("    (none)\n");
        }
        
        parser = parser->next;
    }
    printf("--------------------------------------\n");
}
void	parser(t_data *data)
{
	if (!data || !data->expander)
		return ;
	if (!syntax_control(data->expander))
		return ;
	all_tokens_parse(data, data->expander);
	//print_parser(data->parser);
	executor(data);
	free_parser(data->parser);
	data->parser = NULL;
}
