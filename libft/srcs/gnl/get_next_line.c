/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 16:31:04 by rdalal            #+#    #+#             */
/*   Updated: 2024/09/25 19:34:55 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_extract_line(char *str)
{
	int		i;
	char	*line;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = ft_calloc_gnl(i + (str[i] == '\n') + 1, sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] && str[i] == '\n')
		line[i++] = '\n';
	return (line);
}

char	*ft_get_remaining(char *str)
{
	int		i;
	int		j;
	char	*dup;

	i = 0;
	j = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free (str);
		return (NULL);
	}
	dup = ft_calloc_gnl((ft_strlen_gnl(str) - i + 1), sizeof(char));
	if (!dup)
		return (free(str), NULL);
	i += 1;
	while (str[i])
	{
		dup[j] = str[i];
		j++;
		i++;
	}
	free (str);
	return (dup);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer;

	if (fd < 0 || read(fd, 0, 0) < 0 || BUFFER_SIZE <= 0)
	{
		if (buffer)
		{
			free (buffer);
			buffer = NULL;
		}
		return (NULL);
	}
	buffer = ft_read(buffer, fd);
	if (!buffer)
		return (NULL);
	line = ft_extract_line(buffer);
	if (!line)
		return (free(buffer), buffer = NULL);
	buffer = ft_get_remaining(buffer);
	return (line);
}
