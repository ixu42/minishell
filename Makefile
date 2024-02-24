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

RL_DIR = ~/.brew/opt/readline/lib

LIBFT = $(LIBFT_DIR)/libft.a

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c \
		parser/constructors.c \
		parser/parseexec.c \
		parser/parsecmd.c \
		parser/parsing_utils.c \
		runcmd.c 
#		runcmd_old.c 

OBJS = $(SRCS:.c=.o)

all: $(NAME)

#$(NAME): $(OBJS) $(LIBFT)
#	cc -o $(NAME) $(OBJS) $(LIBFT) -L$(RL_DIR) -lreadline -L$(LIBFT_DIR) -lft
$(NAME): $(OBJS) $(LIBFT)
	cc -o $(NAME) $(OBJS) $(LIBFT) -L$(RL_DIR) -lreadline -I ~/.brew/opt/readline/include -L ~/.brew/opt/readline/lib -L$(LIBFT_DIR) -lft

%.o: %.c
	cc -c $< -o $@ -I ~/.brew/opt/readline/include
	#cc $(CFLAGS) -c $< -o $@ -I ~/.brew/opt/readline/include

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) fclean
 
fclean: clean
	rm -f $(NAME)

re: fclean all
