/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:52:27 by gmechaly          #+#    #+#             */
/*   Updated: 2025/03/16 17:09:27 by gmechaly         ###   ########.fr       */
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

int	new_input_len(char *input, t_envp *env)
{
	int		i;
	int		add_len;
	int		code;
	char	*var_name;
	char	*var_value;

	i = 0;
	add_len = 0;
	code = 0;
	while (i >= 0 && input[i])
	{
		if (input[i] && input[i] == '\"')
			code = (code + 1) % 2;
		if (input[i] && input[i] == '$' && input[i + 1] && !is_quote(input[i + 1]))
		{
			i++;
			if (input[i] == '?')
				add_len += ft_strlen(ft_itoa(g_exit_status));
			else
			{
				var_name = get_var_name(&input[i]);
				var_value = my_getenv(var_name, &env);
				if (var_value == NULL)
					return (free(var_name), -1);
				add_len += ft_strlen(var_value);
				i += ft_strlen(var_name);
				free(var_name);
			}
		}
		if (input[i] && input[i] == '\'' && code == 0)
			i += ft_search_unquote(&input[i], '\'');
		if (input[i] != '\0')
			i++;
	}
	return (i + add_len);
}

void	replace_var_by_value(char *input, char *ninput, int *i, int *j, t_envp *env)
{
	char	*vname;
	char	*vval;

	if (input[(*i) + 1] == '?')
	{
		vval = ft_itoa(g_exit_status);
		(*i) += 2;
	}
	else
	{
		vname = get_var_name(&input[(*i) + 1]);
		vval = my_getenv(vname, &env);
		*i += ft_strlen(vname) + 1;
		free(vname);
	}
	ninput[*j] = '\0';
	if (vval)
	{
		ft_strlcat(ninput, vval, ft_strlen(ninput) + ft_strlen(vval) + 1);
		*j += ft_strlen(vval);
	}
}

char	*expand_variables(char *input, t_envp *env)
{
	char	*ninput;
	int		i;
	int		j;

	i = new_input_len(input, env);
	if (i > 0)
		ninput = (char *)malloc(sizeof(char) * (i + 1));
	if (i <= 0 || ninput == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (is_quote(input[i]) && input[i + 1])
			copy_quote(input, ninput, &i, &j, env);
		else if (input[i] == '$' && is_quote(input[i + 1]))
			handle_quote_after_dollar(input, ninput, &i, &j);
		else if (input[i] == '$' && input[i + 1])
			replace_var_by_value(input, ninput, &i, &j, env);
		else if (input[i] != '\'')
			ninput[j++] = input[i++];
		else
			break ;
	}
	ninput[j] = '\0';
	return (ninput);
}

// int	main(void)
// {
// 	char	*ninput;

// 	ninput = expand_variables("echo $HOME \"$HOME\" $\"HOME\"");
// 	printf("%s\n", ninput);
// 	free(ninput);
// 	return (0);
// }
