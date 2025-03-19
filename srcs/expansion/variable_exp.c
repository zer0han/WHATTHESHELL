/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:52:27 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/19 17:51:44 by gmechaly         ###   ########.fr       */
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
	while (input_i[start + i] && !is_space(input_i[start + i]) \
		&& !is_quote(input_i[start + i]) && input_i[start + i] != '<' \
		&& input_i[start + i] != '>' && input_i[start + i] != '|')
		i++;
	var_name = (char *)malloc(sizeof(char) * (i + 1));
	i = start;
	while (input_i[i] && !is_space(input_i[i]) && !is_quote(input_i[i]) && \
	input_i[start + i] != '<' && input_i[start + i] != '>' \
	&& input_i[start + i] != '|')
	{
		var_name[i] = input_i[i];
		i++;
	}
	var_name[i] = '\0';
	return (var_name);
}

static int	get_var_len(char *input, int *i, int *add_len, t_envp *env)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (input[*i] == '?')
		*add_len += ft_strlen(ft_itoa(g_exit_status));
	else
	{
		var_name = get_var_name(&input[*i]);
		var_value = my_getenv(var_name, &env);
		if (var_value == NULL)
			return (free(var_name), -1);
		*add_len += ft_strlen(var_value);
		*i += ft_strlen(var_name);
		free(var_name);
	}
	return (0);
}

int	new_input_len(char *input, t_envp *env)
{
	int		i;
	int		add_len;
	int		code;

	i = 0;
	add_len = 0;
	code = 0;
	while (i >= 0 && input[i])
	{
		if (input[i] && input[i] == '\"')
			code = (code + 1) % 2;
		if (input[i] && input[i] == '$' && \
			input[i + 1] && !is_quote(input[i + 1]))
		{
			if (get_var_len(input, &i, &add_len, env) < 0)
				return (-1);
		}
		if (input[i] && input[i] == '\'' && code == 0)
			i += ft_search_unquote(&input[i], '\'');
		if (input[i] != '\0')
			i++;
	}
	return (i + add_len);
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
	}
}

char	*expand_variables(char *input, t_envp *env)
{
	t_vexp	*data;

	data->i = new_input_len(input, env);
	if (data->i > 0)
		data->n_ipt = (char *)malloc(sizeof(char) * (data->i + 1));
	if (data->i <= 0 || data->n_ipt == NULL)
		return (NULL);
	data->i = 0;
	data->j = 0;
	data->o_ipt = input;
	while (data->o_ipt[data->i])
	{
		if (is_quote(data->o_ipt[data->i]) && data->o_ipt[data->i + 1])
			copy_quote(data, env);
		else if (data->o_ipt[data->i] == '$' && is_quote(data->o_ipt[data->i + 1]))
			handle_quote_after_dollar(data);
		else if (data->o_ipt[data->i] == '$' && data->o_ipt[data->i + 1])
			replace_var_by_value(data, env);
		else if (data->o_ipt[data->i] != '\'')
			data->n_ipt[data->j++] = data->o_ipt[data->i++];
		else
			break ;
	}
	data->n_ipt[data->j] = '\0';
	return (data->n_ipt);
}

// int	main(void)
// {
// 	char	*ninput;

// 	ninput = expand_variables("echo $HOME \"$HOME\" $\"HOME\"");
// 	printf("%s\n", ninput);
// 	free(ninput);
// 	return (0);
// }
