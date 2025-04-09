/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 00:57:14 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 01:33:01 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*multi_line_heredoc(t_redir *redir, int fd, t_envp *env)
{
	int		i;
	char	*line;
	char	*exp;

	i = 0;
	while (redir->delimiter[i])
	{
		line = readline("> ");
		if (line && !redir->delimiter[i + 1] && \
		ft_strcmp(line, redir->delimiter[i]))
		{
			exp = expand_variables(line, env);
			if (!exp)
				return (free(line), NULL);
			write(fd, exp, ft_strlen(exp));
			write(fd, "\n", 1);
			free(exp);
		}
		else if (!line || !ft_strcmp(line, redir->delimiter[i]))
			i++;
		free(line);
	}
	return (redir);
}
