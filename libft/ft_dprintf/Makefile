# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/21 10:44:52 by ixu               #+#    #+#              #
#    Updated: 2024/02/17 17:13:29 by ixu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the program
NAME = libftprintf.a

# Directory locations
LIBFT_DIR = libft/

# Source files
SRCS = ft_dprintf.c \
		flags.c \
		print_chars.c \
		print_str.c \
		print_dec.c \
		print_udec.c \
		print_hex.c \
		print_ptr.c \
		print_utils.c \
		ft_printf_utils.c

# Object files including paths
OBJS = $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror

LIBFT_NAME = libft.a

LIBFT = $(addprefix $(LIBFT_DIR), $(LIBFT_NAME))

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIBFT_DIR)
	cp $(LIBFT) $(NAME)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) fclean
 
fclean: clean
	rm -f $(NAME)

re: fclean all