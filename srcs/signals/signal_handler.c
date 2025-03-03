/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:41:10 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/03 23:35:34 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	clear_rl_line(void)
{
	rl_replace_line("", 0);
	rl_on_new_line();
}

static void	handle_sigint(int sig)
{
	(void)sig;
	printf("\n");
	clear_rl_line();
	if (g_exit_status == 0)
		rl_redisplay();
}

void	signals(void)
{
	sigaction(SIGINT, &handle_sigint, NULL);
	sigaction(SIGQUIT, SIG_IGN, NULL);
}
