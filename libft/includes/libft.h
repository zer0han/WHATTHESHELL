/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 20:05:33 by rdalal            #+#    #+#             */
/*   Updated: 2024/11/19 15:26:51 by rdalal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <string.h>

int		ft_isalpha(int c);

int		ft_toupper(int c);

int		ft_isdigit(int c);

int		ft_tolower(int c);

int		ft_isalnum(int c);

int		ft_isascii(int c);

int		ft_isprint(int c);

int		ft_atoi(const char *str);

int		ft_word_count(const char *str, char c);

int		ft_memcmp(const void *s1, const void *s2, size_t n);

int		ft_strncmp(const char *str1, const char *str2, size_t n);

size_t	ft_strlen(char const *str);

size_t	ft_strlcat(char *dst, const char *src, size_t size);

size_t	ft_strlcpy(char *dst, char *src, size_t l);

void	ft_bzero(void *s, size_t n);

void	*ft_calloc(size_t nmemb, size_t size);

void	*ft_memchr(const void *str, int c, size_t n);

void	*ft_memcpy(void *dest, const void *src, size_t n);

void	*ft_memmove(void *dest, const void *src, size_t n);

void	*ft_memset(void *s, int c, size_t n);

void	ft_putchar_fd(char c, int fd);

void	ft_putendl_fd(char *s, int fd);

void	ft_putnbr_fd(int n, int fd);

void	ft_putstr_fd(char *s, int fd);

void	ft_striteri(char *s, void (*f)(unsigned int, char *));

void	free_split(char **split, int word_all);

char	*ft_itoa(int n);

char	**ft_split(char const *s, char c);

char	*ft_strchr(char *str, int c);

char	*ft_strdup(char *s);

char	*ft_strjoin(char const *s1, char const *s2);

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));

char	*ft_strnstr(const char *str, const char *find, size_t len);

char	*ft_strrchr(char *str, int c);

char	*ft_strtrim(char const *s1, char const *set);

char	*ft_substr(char const *s, unsigned int start, size_t len);

long	ft_atol(const char *s);

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);

void	ft_lstadd_front(t_list **lst, t_list *new);

int		ft_lstsize(t_list *lst);

t_list	*ft_lstlast(t_list *lst);

void	ft_lstadd_back(t_list **lst, t_list *new);

void	ft_lstdelone(t_list *lst, void (*del)(void *));

void	ft_lstclear(t_list **lst, void (*del)(void *));

void	ft_lstiter(t_list *lst, void (*f)(void *));

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
