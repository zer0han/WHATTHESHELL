/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp_tools.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 16:26:32 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/19 17:57:29 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quote_after_dollar(t_vexp *data)
{
	if (data->o_ipt[data->i + 1] == '\'')
		data->i++;
	else if (data->o_ipt[data->i + 1] == '\"')
	{
		data->n_ipt[data->j] = data->o_ipt[data->i];
		data->i += 2;
		data->j++;
		while (data->o_ipt[data->i] && data->o_ipt[data->i] != '<' \
		&& data->o_ipt[data->i] != '>' && data->o_ipt[data->i] != '|' \
		&& data->o_ipt[data->i] != '\"')
		{
			data->n_ipt[data->j] = data->o_ipt[data->i];
			data->i++;
			data->j++;
		}
		data->n_ipt[data->j] = '\0';
		if (data->o_ipt[data->i] == '\"')
			data->i++;
	}
}

void	copy_quote(t_vexp *data, t_envp *env)
{
	if (data->o_ipt[data->i] == '\'')
		copy_squote(data);
	else if (data->o_ipt[data->i] == '\"')
		copy_dquote(data, env);
}

void	copy_dquote(t_vexp *data, t_envp *env)
{
	if (data->o_ipt[data->i] == '\"')
		data->n_ipt[data->j++] = data->o_ipt[data->i++];
	while (data->o_ipt[data->i] && data->o_ipt[data->i] != '\"')
	{
		if (data->o_ipt[data->i] == '$' && data->o_ipt[data->i + 1])
			replace_var_by_value(data, env);
		else
			data->n_ipt[data->j++] = data->o_ipt[data->i++];
	}
	if (data->o_ipt[data->i] == '\"')
		data->n_ipt[data->j++] = data->o_ipt[data->i++];
}

void	copy_squote(t_vexp *data)
{
	if (data->o_ipt[data->i] == '\'')
		data->n_ipt[data->j++] = data->o_ipt[data->i++];
	while (data->o_ipt[data->i] != '\'' && data->o_ipt[data->i])
		data->n_ipt[data->j++] = data->o_ipt[data->i++];
	if (data->o_ipt[data->i] == '\'')
		data->n_ipt[data->j++] = data->o_ipt[data->i++];
}

char	*my_getenv(char *var_name, t_envp **env)
{
	t_envp	*node;

	node = *env;
	if (!valid_id(var_name))
		return (NULL);
	while (node)
	{
		if (!ft_strncmp(node->str, var_name, ft_strlen(var_name)))
			return (ft_strchr(node->str, '=') + 1);
		node = node->next;
	}
	return (NULL);
}
