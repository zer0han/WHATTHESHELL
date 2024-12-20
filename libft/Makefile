# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/24 16:56:26 by rdalal            #+#    #+#              #
#    Updated: 2024/11/19 15:30:43 by rdalal           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

SRCS_LIBFT	=	ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
				ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
				ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
				ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
				ft_atoi.c ft_calloc.c ft_strdup.c ft_atol.c \
				ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
				ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
				ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
				ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c \
				ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c \

SRCS_GNL	=	get_next_line_utils.c get_next_line.c

SRCS_PRINTF	=	ft_print_utils.c ft_printf_hexa.c ft_printf_int.c \
				ft_printf_ptr.c ft_printf_unsigned.c ft_printf.c

SRCS_LIBFT_PATH	=	srcs/Libft/
SRCS_GNL_PATH	=	srcs/gnl/
SRCS_PRINTF_PATH	=	srcs/ft_printf/

SRCS	=	$(addprefix $(SRCS_LIBFT_PATH), $(SRCS_LIBFT)) \
			$(addprefix $(SRCS_GNL_PATH), $(SRCS_GNL)) \
			$(addprefix $(SRCS_PRINTF_PATH), $(SRCS_PRINTF))

CC	=	gcc

CFLAGS =	-Wall -Wextra -Werror

AR	=	ar rcs

HEAD	=	includes/
			
OBJS	=	$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
		$(AR) $(NAME) $(OBJS)

.c.o:
		$(CC) $(CFLAGS) -c $< -o $@ -I$(HEAD)

clean: 
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean