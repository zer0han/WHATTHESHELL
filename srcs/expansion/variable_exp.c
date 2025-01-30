/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_exp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:52:27 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/30 16:51:09 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_var_name(char *input_i)
{
	int		i;
	char	*var_name;

	i = 0;
	while (!is_space(input_i[i]))
		i++;
	var_name = malloc(sizeof(char) * (i + 1));
	i = 0;
	while (!is_space(input_i[i]))
	{
		var_name[i] = input_i[i];
		i++;
	}
	var_name[i] = '\0';
	return (var_name);
}

int	new_input_len(char *input)
{
	int		i;
	int		add_len;
	char	*var_name;
	char	*var_value;

	i = 0;
	add_len = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1])
		{
			i++;
			var_name = get_var_name(&input[i]);
			var_value = getenv(var_name);
			if (var_value == NULL)
				return (-1);
			add_len += ft_strlen(var_value);
			i += ft_strlen(var_name);
			free(var_name);
		}
		i++;
	}
	return (i + add_len);
}

void	replace_var_by_value(char *input, char *ninput, int *i, int *j)
{
	char	*vname;
	char	*vval;

	vname = get_var_name(&input[(*i) + 1]);
	vval = getenv(vname);
	ft_strlcat(ninput, vval, ft_strlen(ninput) + ft_strlen(vval) + 1);
	*j += ft_strlen(vval);
	*i += ft_strlen(vname) + 1;
	free(vname);
}

char	*expand_variables(char *input)
{
	char	*ninput;
	int		i;
	int		j;

	ninput = malloc(sizeof(char) * new_input_len(input));
	if (ninput == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1])
			replace_var_by_value(input, ninput, &i, &j);
		ninput[j++] = input[i++];
		ninput[j] = '\0';
	}
	return (ninput);
}

// int	main(void)
// {
// 	char	*ninput;

// 	ninput = expand_variables("echo $PATH test");
// 	printf("%s\n", ninput);
// 	free(ninput);
// 	return (0);
// }