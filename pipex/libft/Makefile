# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/23 15:42:20 by ixu               #+#    #+#              #
#    Updated: 2023/11/08 15:12:20 by ixu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SOURCES := 	nbr/ft_atoi.c \
			nbr/ft_itoa.c \
			\
			mem/ft_bzero.c \
			mem/ft_calloc.c \
			mem/ft_memchr.c \
			mem/ft_memcmp.c \
			mem/ft_memcpy.c \
			mem/ft_memmove.c \
			mem/ft_memset.c \
			\
			char/ft_isalnum.c \
			char/ft_isalpha.c \
			char/ft_isascii.c \
			char/ft_isdigit.c \
			char/ft_isprint.c \
			char/ft_tolower.c \
			char/ft_toupper.c \
			\
			str/ft_split.c \
			str/ft_split_charset.c \
			str/ft_split_pipex.c \
			str/ft_strchr.c \
			str/ft_strdup.c \
			str/ft_striteri.c \
			str/ft_strjoin.c \
			str/ft_strlcat.c \
			str/ft_strlcpy.c \
			str/ft_strlen.c \
			str/ft_strmapi.c \
			str/ft_strcmp.c \
			str/ft_strncmp.c \
			str/ft_strnstr.c \
			str/ft_strrchr.c \
			str/ft_strtrim.c \
			str/ft_substr.c \
			\
			file/ft_putchar_fd.c \
			file/ft_putendl_fd.c \
			file/ft_putnbr_fd.c \
			file/ft_putstr_fd.c \
			file/get_next_line.c \
			file/get_next_line_utils.c \
			\
			lst/ft_lstnew.c \
			lst/ft_lstadd_front.c \
			lst/ft_lstsize.c \
			lst/ft_lstlast.c \
			lst/ft_lstadd_back.c \
			lst/ft_lstdelone.c \
			lst/ft_lstclear.c \
			lst/ft_lstiter.c \
			lst/ft_lstmap.c

OBJECTS := $(SOURCES:.c=.o)

NAME = libft.a

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $(NAME) $(OBJECTS)

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
 
fclean: clean
	rm -f $(NAME)
	
re: fclean all