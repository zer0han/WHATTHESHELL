/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 19:51:08 by gmechaly          #+#    #+#             */
/*   Updated: 2025/04/09 00:08:18 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *input_i)
{
	int		i;
	int		start;
	char	*var_name;

	i = 0;
	start = 0;
	while (input_i[i + start] == '$' || is_quote(input_i[i + start]))
		start++;
	while (input_i[start + i] && (ft_isalnum(input_i[start + i]) || \
		input_i[start + i] == '_'))
		i++;
	var_name = (char *)malloc(sizeof(char) * (i + 1));
	if (!var_name)
		return (NULL);
	i = start;
	while (input_i[i] && (ft_isalnum(input_i[i]) || input_i[i] == '_'))
	{
		var_name[i] = input_i[i];
		i++;
	}
	var_name[i] = '\0';
	return (var_name);
}

int	get_var_len(char *input, int *i, int *add_len, t_envp *env)
{
	char	*var_name;
	char	*var_value;
	char	*exit_status;

	(*i)++;
	exit_status = ft_itoa(g_exit_status);
	if (!exit_status)
		return (-1);
	if (input[*i] == '?')
		*add_len += ft_strlen(exit_status);
	else
	{
		var_name = get_var_name(&input[*i]);
		var_value = my_getenv(var_name, &env);
		if (var_value == NULL)
			return (free(exit_status), free(var_name), -1);
		*add_len += ft_strlen(var_value);
		*i += ft_strlen(var_name);
		free(var_name);
	}
	free(exit_status);
	return (0);
}

void	replace_var_by_value(t_vexp *data, t_envp *env)
{
	char	*vname;
	char	*vval;

	if (data->o_ipt[data->i + 1] == '?')
	{
		vval = ft_itoa(g_exit_status);
		data->i += 2;
	}
	else
	{
		vname = get_var_name(&data->o_ipt[data->i + 1]);
		vval = my_getenv(vname, &env);
		data->i += ft_strlen(vname) + 1;
		free(vname);
	}
	data->n_ipt[data->j] = '\0';
	if (vval)
	{
		ft_strlcat(data->n_ipt, vval, \
			ft_strlen(data->n_ipt) + ft_strlen(vval) + 1);
		data->j += ft_strlen(vval);
		if (data->o_ipt[data->i - 1] == '?')
			free(vval);
	}
}
