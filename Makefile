# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/09 11:00:23 by ixu               #+#    #+#              #
#    Updated: 2024/02/16 11:26:51 by ixu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFT_DIR = libft

RL_DIR = ~/.brew/opt/readline/lib

LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c \
		parcer-main.c \
		runcmd.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	cc -o $(NAME) $(OBJS) $(LIBFT) -L$(RL_DIR) -lreadline -L$(LIBFT_DIR) -lft

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
 
fclean: clean
	rm -f $(NAME)
	rm -f $(LIBFT)

re: fclean all