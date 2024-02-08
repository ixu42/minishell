# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/09 11:00:23 by ixu               #+#    #+#              #
#    Updated: 2024/02/09 11:09:15 by ixu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c \

# SRCS_BONUS = 

OBJS = $(SRCS:.c=.o)

# OBJS_BONUS = $(SRCS_BONUS:.c=.o)

NAME = minishell

LIBFT_DIR = libft

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C libft
	cc -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

# .base:
# 	rm -f .bonus
# 	touch .base
# 
# bonus: .bonus
# 
# .bonus: $(OBJS_BONUS)
# 	$(MAKE) -C libft
# 	cc -o $(NAME) $(OBJS_BONUS) -L$(LIBFT_DIR) -lft
# 	rm -f .base
# 	touch .bonus

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	# rm -f $(OBJS) $(OBJS_BONUS)
	$(MAKE) -C libft clean
	# rm -f .bonus
	# rm -f .base
 
fclean: clean
	rm -f $(NAME)
	rm -f libft/libft.a

re: fclean all