# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/23 15:42:20 by ixu               #+#    #+#              #
#    Updated: 2024/03/19 11:28:59 by ixu              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the program
NAME = libft.a

# Directory locations
NBR_DIR = nbr/
MEM_DIR = mem/
CHAR_DIR = char/
STR_DIR = str/
FILE_DIR = file/
LIST_DIR = lst/
FT_DPRINTF_DIR = ft_dprintf/
LIBFT_DIR = libft/

# Source files names by folders
NBR_FILES = ft_atoi.c \
			ft_strtol.c \
			ft_itoa.c
MEM_FILES = ft_bzero.c \
			ft_calloc.c \
			ft_memchr.c \
			ft_memcmp.c \
			ft_memcpy.c \
			ft_memmove.c \
			ft_memset.c
CHAR_FILES = ft_isalnum.c \
			ft_isalpha.c \
			ft_isascii.c \
			ft_isdigit.c \
			ft_isprint.c \
			ft_tolower.c \
			ft_toupper.c
STR_FILES = ft_split.c \
			ft_split_charset.c \
			ft_split_pipex.c \
			ft_strchr.c \
			ft_strdup.c \
			ft_striteri.c \
			ft_strjoin.c \
			ft_strlcat.c \
			ft_strlcpy.c \
			ft_strlen.c \
			ft_strmapi.c \
			ft_strcmp.c \
			ft_strncmp.c \
			ft_strnstr.c \
			ft_strrchr.c \
			ft_strtrim.c \
			ft_substr.c
FILE_FILES = ft_putchar_fd.c \
			ft_putendl_fd.c \
			ft_putnbr_fd.c \
			ft_putstr_fd.c \
			get_next_line.c \
			get_next_line_utils.c
LIST_FILES = ft_lstnew.c \
			ft_lstadd_front.c \
			ft_lstsize.c \
			ft_lstlast.c \
			ft_lstadd_back.c \
			ft_lstdelone.c \
			ft_lstclear.c \
			ft_lstiter.c \
			ft_lstmap.c
FT_DPRINTF_FILES = ft_dprintf.c \
					flags.c \
					print_chars.c \
					print_str.c \
					print_dec.c \
					print_udec.c \
					print_hex.c \
					print_ptr.c \
					print_utils.c \
					ft_printf_utils.c

SRCS = $(addprefix $(NBR_DIR), $(NBR_FILES)) \
		$(addprefix $(MEM_DIR), $(MEM_FILES)) \
		$(addprefix $(CHAR_DIR), $(CHAR_FILES)) \
		$(addprefix $(STR_DIR), $(STR_FILES)) \
		$(addprefix $(FILE_DIR), $(FILE_FILES)) \
		$(addprefix $(LIST_DIR), $(LIST_FILES)) \
		$(addprefix $(FT_DPRINTF_DIR), $(FT_DPRINTF_FILES))

OBJS := $(SRCS:.c=.o)

CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
 
fclean: clean
	@rm -f $(NAME)
	
re: fclean all