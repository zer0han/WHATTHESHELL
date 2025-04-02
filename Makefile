# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdalal <rdalal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/05 20:07:53 by rdalal            #+#    #+#              #
#    Updated: 2025/04/01 20:45:31 by rdalal           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

HEAD	= includes/

CC		= cc

CFLAGS	= -Wextra -Wall -Werror

READLINE_FLAGS	= -lreadline

SRCS_BI		=	builtin_echo.c builtin_exit.c builtin_export.c \
				builtin_unset.c builtins.c extrnl_cmds.c

SRCS_EXEC	=	dispatch.c exec_helper.c exec.c

SRCS_EXP	=	variable_exp.c variable_exp_tools.c var_tools.c

SRCS_PARSE	=	assign_tokens.c custom_libft.c parse_tokens.c \
				split_for_tokens.c split_utils.c struct_utils.c \
				tokenize.c tools.c free_chars.c free_structs.c main.c

SRCS_PIPE	=	pipe_helper.c pipes.c

SRCS_REDIR	=	redirection_handler.c redirection.c

SRCS_SIG	=	signal_handler.c

SRCS_UTILS	=	error_handling.c

SRCS_BI_PATH	= srcs/builtins/
SRCS_EXEC_PATH	= srcs/exec/
SRCS_EXP_PATH	= srcs/expansion/
SRCS_PARSE_PATH	= srcs/parsing/
SRCS_PIPE_PATH	= srcs/pipe/
SRCS_REDIR_PATH	= srcs/redirection/
SRCS_SIG_PATH	= srcs/signals/
SRCS_UTILS_PATH	= srcs/utils/

SRCS	=	$(addprefix $(SRCS_BI_PATH), $(SRCS_BI)) \
			$(addprefix $(SRCS_EXEC_PATH), $(SRCS_EXEC)) \
			$(addprefix $(SRCS_EXP_PATH), $(SRCS_EXP)) \
			$(addprefix $(SRCS_PARSE_PATH), $(SRCS_PARSE)) \
			$(addprefix $(SRCS_PIPE_PATH), $(SRCS_PIPE)) \
			$(addprefix $(SRCS_REDIR_PATH), $(SRCS_REDIR)) \
			$(addprefix $(SRCS_SIG_PATH), $(SRCS_SIG)) \
			$(addprefix $(SRCS_UTILS_PATH), $(SRCS_UTILS)) \
			srcs/main.c

LIBFT_H_PATH	= libft/includes/

LIBFT_PATH	=	libft/

LIBFT	= $(LIBFT_PATH)libft.a

OBJS	= $(SRCS:.c=.o)

all: $(NAME)
		$(print_flag)

$(LIBFT):
			@$(MAKE) -C $(LIBFT_PATH) -s

$(NAME):	$(OBJS) $(LIBFT) $(HEAD)
			@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(READLINE_FLAGS)

.c.o:
			@$(CC) $(CFLAGS) -c $< -o $@ -I$(HEAD) -I$(LIBFT_H_PATH)

clean:	libft-clean root-clean

libft-clean:
			@$(MAKE) -C $(LIBFT_PATH) clean
	
root-clean:
			@rm -f $(OBJS)

fclean: libft-fclean root-fclean

libft-fclean:
			@$(MAKE) -C $(LIBFT_PATH) fclean

root-fclean: root-clean
			@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libft libft-clean libft-fclean root-clean root-fclean

define print_flag
	@echo ".☆.｡.:*・°･.｡*･.｡* .☆.｡.:*.☆.｡.:*・°･.｡*･.｡*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*"
	@cat ascii_art.txt
	@echo ""
	@echo ".☆.｡.:*・°･.｡*･.｡* .☆.｡.:*.☆.｡.:*・°･.｡*･.｡*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*.☆.｡.:*"
endef