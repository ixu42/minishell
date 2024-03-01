# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/09 11:00:23 by ixu               #+#    #+#              #
#    Updated: 2024/02/24 12:06:36 by apimikov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT_DIR = libft

RL_DIR = $(HOME)/.brew/opt/readline/lib

RL_HEADER_DIR = $(HOME)/.brew/opt/readline/include/readline

LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c \
		parser/constructors.c \
		parser/parseexec.c \
		parser/parsecmd.c \
		parser/parsing_utils.c \
		runcmd.c \
		get_cmd_path.c \
		env.c \
		builtins/builtins.c \
		builtins/echo.c \
		builtins/directory.c \
		builtins/export.c \
		builtins/unset.c \
		builtins/env.c \
		builtins/exit.c \
		error.c \
		free.c
#		runcmd_old.c 

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	cc -o $(NAME) $(OBJS) $(LIBFT) -L$(RL_DIR) -lreadline -L$(LIBFT_DIR) -lft

%.o: %.c
	cc -c $< -o $@ -I$(RL_HEADER_DIR)
	#cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) fclean
 
fclean: clean
	rm -f $(NAME)

re: fclean all
