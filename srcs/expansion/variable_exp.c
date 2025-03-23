/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:52:27 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/23 16:13:05 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static t_vexp	*init_data(char *input, t_envp *env)
{
	t_vexp	*data;

	data = malloc(sizeof(t_vexp));
	if (data == NULL)
		return (NULL);
	data->i = new_input_len(input, env);
	if (data->i > 0)
		data->n_ipt = (char *)malloc(sizeof(char) * (data->i + 1));
	if (data->i <= 0 || data->n_ipt == NULL)
		return (NULL);
	data->i = 0;
	data->j = 0;
	data->o_ipt = input;
	return (data);
}

char	*expand_variables(char *input, t_envp *env)
{
	t_vexp	*data;
	char	*new_input;

	data = init_data(input, env);
	if (data == NULL)
		return (NULL);
	while (data->o_ipt[data->i])
	{
		if (is_quote(data->o_ipt[data->i]) && data->o_ipt[data->i + 1])
			copy_quote(data, env);
		else if (data->o_ipt[data->i] == '$' && \
				is_quote(data->o_ipt[data->i + 1]))
			handle_quote_after_dollar(data);
		else if (data->o_ipt[data->i] == '$' && data->o_ipt[data->i + 1])
			replace_var_by_value(data, env);
		else if (data->o_ipt[data->i] != '\'')
			data->n_ipt[data->j++] = data->o_ipt[data->i++];
		else
			break ;
	}
	data->n_ipt[data->j] = '\0';
	new_input = ft_strdup(data->n_ipt);
	free(data->n_ipt);
	free(data);
	return (new_input);
}

// int	main(void)
// {
// 	char	*ninput;

// 	ninput = expand_variables("echo $HOME \"$HOME\" $\"HOME\"");
// 	printf("%s\n", ninput);
// 	free(ninput);
// 	return (0);
// }
