# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/12 15:32:27 by ixu               #+#    #+#              #
#    Updated: 2024/01/14 23:56:32 by ixu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = pipex.c \
		utils.c \
		parsing.c \
		free.c

SRCS_BONUS = pipex_bonus.c \
				utils_bonus.c \
				files_bonus.c \
				parsing_bonus.c \
				error_bonus.c \
				free_bonus.c

OBJS = $(SRCS:.c=.o)

OBJS_BONUS = $(SRCS_BONUS:.c=.o)

NAME = pipex

LIBFT_DIR = libft

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) .base
	$(MAKE) -C libft
	cc -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

.base:
	rm -f .bonus
	touch .base

bonus: .bonus

.bonus: $(OBJS_BONUS)
	$(MAKE) -C libft
	cc -o $(NAME) $(OBJS_BONUS) -L$(LIBFT_DIR) -lft
	rm -f .base
	touch .bonus

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OBJS_BONUS)
	$(MAKE) -C libft clean
	rm -f .bonus
	rm -f .base
 
fclean: clean
	rm -f $(NAME)
	rm -f libft/libft.a

re: fclean all