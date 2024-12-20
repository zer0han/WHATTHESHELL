/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 17:31:00 by rdalal            #+#    #+#             */
/*   Updated: 2024/11/19 13:13:18 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_sep(char c, char sep)
{
	if (c == sep || c == '\0')
		return (1);
	return (0);
}

int	ft_word_count(const char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i] && ft_sep(str[i], c))
		i++;
	while (str[i] != '\0')
	{
		if (!ft_sep(str[i], c) && ft_sep(str[i + 1], c))
			count++;
		i++;
	}
	return (count);
}

static char	*ft_sepstrdup(char const *s, char c)
{
	char	*dest;
	int		i;
	int		j;

	j = 0;
	while (!ft_sep(s[j], c))
		j++;
	dest = (char *)malloc(j + 1);
	if (!dest)
		return (NULL);
	i = 0;
	while (i < j)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	free_split(char **split, int word_all)
{
	int	i;

	i = 0;
	while (i < word_all)
	{
		if (split[i])
		{
			free (split[i]);
			split[i] = NULL;
		}
		i++;
	}
	free (split);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		word_index;

	i = 0;
	word_index = 0;
	result = (char **)malloc((ft_word_count(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (s[i])
	{
		if (ft_sep(s[i], c))
			i++;
		else
		{
			result[word_index] = ft_sepstrdup(&s[i], c);
			if (!result[word_index])
				return (free_split(result, word_index), NULL);
			i += ft_strlen(result[word_index]);
			word_index++;
		}
	}
	result[word_index] = NULL;
	return (result);
}

/*#include <stdio.h>

int main(void) {
    char **result;
    int i;

    // Test case 1
    char str1[] = "Hello, world, this is, a test";
    char delimiter1 = ',';
    printf("Test case 1:\nInput: \"%s\" Delimiter: '%c'\n", str1, delimiter1);
    result = ft_split(str1, delimiter1);
    if (result) {
        i = 0;
        while (result[i]) {
            printf("Result[%d]: \"%s\"\n", i, result[i]);
            i++;
        }
        free_split(result, i);
    } else {
        printf("Memory allocation failed\n");
    }

    // Test case 2
    char str2[] = "Another test case with spaces";
    char delimiter2 = ' ';
    printf("\nTest case 2:\nInput: \"%s\" Delimiter: '%c'\n", str2, delimiter2);
    result = ft_split(str2, delimiter2);
    if (result) {
        i = 0;
        while (result[i]) {
            printf("Result[%d]: \"%s\"\n", i, result[i]);
            i++;
        }
        free_split(result, i);
    } else {
        printf("Memory allocation failed\n");
    }

    // Test case 3
    char str3[] = "One-more-test-case";
    char delimiter3 = '-';
    printf("\nTest case 3:\nInput: \"%s\" Delimiter: '%c'\n", str3, delimiter3);
    result = ft_split(str3, delimiter3);
    if (result) {
        i = 0;
        while (result[i]) {
            printf("Result[%d]: \"%s\"\n", i, result[i]);
            i++;
        }
        free_split(result, i);
    } else {
        printf("Memory allocation failed\n");
    }

	// Test case 4
    char str4[] = "";
    char delimiter4 = 'y';
    printf("\nTest case 4:\nInput: \"%s\" Delimiter: '%c'\n", str4, delimiter4);
    result = ft_split(str4, delimiter4);
    if (result) {
        i = 0;
        while (result[i]) {
            printf("Result[%d]: \"%s\"\n", i, result[i]);
            i++;
        }
        free_split(result, i);
    } else {
        printf("Memory allocation failed\n");
    }

    return 0;
}*/