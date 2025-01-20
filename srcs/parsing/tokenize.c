/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmechaly <gmechaly@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:40:07 by gmechaly          #+#    #+#             */
/*   Updated: 2025/01/20 20:06:26 by gmechaly         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*		**	RAPPEL	**
*	typedef struct s_token {
*	char			*input;
*	char			*type;
*	int				value;
*	struct s_token	*left;
*	struct s_token	*right;
*	}			t_token;
*/

	/*UNFINISHED*/
char	*ft_parse_token(char *token)
{
	if (!ft_strncmp(token, "echo", 4) || !ft_strncmp(token, "cd", 2) \
		|| !ft_strncmp(token, "pwd", 3) || !ft_strncmp(token, "export", 6) \
		|| !ft_strncmp(token, "unset", 5) || !ft_strncmp(token, "env", 3) \
		|| !ft_strncmp(token, "exit", 4) || getenv(token) != NULL)
		return ("cmd");
	else if (token[0] == '-')
		return ("arg");
	else if (!ft_strncmp(token, "|", 1))
		return ("pipe");
	else if (!ft_strncmp(token, "<", 1) || !ft_strncmp(token, ">", 1))
		return ("redirection");
	else if (!ft_strncmp(token, "<<", 2))
		return ("delimiter");
	else if (!ft_strncmp(token, ">>", 2))
		return ("append mode");
	else if (token[0] == '\'')
		return ("single quote");
	else if (token[0] == '\"')
		return ("double quote");
	else if (!ft_strncmp(token, "infile", 6) || !ft_strncmp(token, "outfile", 7)) //this line is temporary
		return ("file");
	else
		return (NULL);
}

t_token	*create_node(t_token **tokens, char *char_token)
{
	t_token	*node;

	node = ft_calloc(1, sizeof(t_token));
	if (node == NULL)
		return (NULL);
	node->input = ft_strdup(char_token);
	// node->type = ft_parse_token(char_token);
	// if (node->type == NULL)
	// 	return (NULL);
	if (!(*tokens))
	{
		tokens = &node;
		node->left = NULL;
	}
	else
	{
		node->left = ft_lastnode(*tokens);
		node->left->right = node;
	}
	node->right = NULL;
	return (*tokens);
}

t_token	*ft_tokenize(char *input)
{
	char	**char_tokens;
	int		i;
	t_token	*tokens;

	char_tokens = ft_split_for_tokens(input);
	if (char_tokens == NULL)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (char_tokens[i] != NULL)
	{
		printf("char_tokens[%d] = %s\n", i, char_tokens[i]);
		tokens = create_node(&tokens, char_tokens[i]);
		if (tokens == NULL)
			return (free_string_tab(&char_tokens[i]), NULL);
		free(char_tokens[i]);
		i++;
	}
	free (char_tokens);
	printf("end of tokenize\n");
	return (tokens);
}
