# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 20:07:53 by rdalal            #+#    #+#              #
#    Updated: 2024/12/20 16:26:39 by rdalal           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

HEAD	= includes/

CC		= gcc

CFLAGS	= -Wextra -Wall -Werror

READLINE_FLAGS	= -lreadline

SRCS_CMDS	=	

SRCS_UTILS	=

SRCS_EXEC	=

SRCS_BI		=

SRCS_CMDS_PATH	= srcs/commands/
SRCS_UTILS_PATH	= srcs/utils/
SRCS_EXEC_PATH	= srcs/exec/
SRCS_BI_PATH	= srcs/builtins/

SRCS	=	$(addprefix $(SRCS_CMDS_PATH), $(SRCS_CMDS)) \
			$(addprefix $(SRCS_UTILS_PATH), $(SRCS_UTILS)) \
			$(addprefix $(SRCS_EXEC_PATH), $(SRCS_EXEC)) \
			$(addprefix $(SRCS_BI_PATH), $(SRCS_BI))

LIBFT_H_PATH	= libft/includes/

LIBFT_PATH	=	libft/

LIBFT	= $(LIBFT_PATH)libft.a

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
			$(MAKE) -C $(LIBFT_PATH)

$(NAME):	$(OBJS) $(LIBFT) $(HEAD)
			$(CC) $(CFLAGS) $(READLINE_FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

.c.o:
			$(CC) $(CFLAGS) -c $< -o $@ -I$(HEAD) -I$(LIBFT_H_PATH)

clean:	libft-clean root-clean

libft-clean:
			$(MAKE) -C $(LIBFT_PATH) clean
	
root-clean:
			rm -f $(OBJS)

fclean: libft-fclean root-fclean

libft-fclean:
			$(MAKE) -C $(LIBFT_PATH) fclean

root-fclean: root-clean
			rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft libft-clean libft-fclean root-clean root-fclean