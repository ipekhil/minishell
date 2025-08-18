/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sude <sude@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:53:20 by hilalipek         #+#    #+#             */
/*   Updated: 2025/08/19 00:21:32 by sude             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_flag = 0;

void	signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);	
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal_flag = 1;
	}
}

void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		close(STDIN_FILENO);
		rl_on_new_line();
		g_signal_flag = 2;
	}
}

void	signal_handlers_main(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	signal_handlers_heredoc(void)
{
	signal(SIGINT, signal_handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
