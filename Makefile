# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/09 11:00:23 by ixu               #+#    #+#              #
#    Updated: 2024/03/19 10:55:06 by apimikov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Name of the program
NAME = minishell

# Directory locations
MAIN_DIR = main/
PARSER_DIR = parser/
STRINGS_DIR = strings/
EXEC_DIR = exec/
BUILTINS_DIR = builtins/
OBJS_DIR = objs/
LIBFT_DIR = libft/
RL_DIR = $(HOME)/.brew/opt/readline/lib
RL_HEADER_DIR = $(HOME)/.brew/opt/readline/include/readline

# Source files by folder
MAIN_FILES = main.c \
				data_init.c \
				data_init_utils.c \
				signals.c \
				signal_handlers.c \
				error.c \
				free.c
PARSER_FILES = constructors_tree.c \
				constructors_aux.c \
				freecmd.c \
				free_exec.c \
				make_ast.c \
				parseexec.c \
				parseline.c \
				parseredirs.c \
				parsestr.c \
				parse_word_singl_var.c \
				parse_heredoc.c \
				parsing_utils.c \
				../remove_before_submission/runcmd_test.c
STRINGS_FILES = str_list_utils.c \
				match.c \
				arraylist.c \
				heapsort_str.c
EXEC_FILES = runcmd.c \
				runcmd_utils.c \
				run_exec.c \
				get_cmd_path.c \
				environment.c \
				heredoc.c
BUILTINS_FILES = builtins.c \
					echo.c \
					directory.c \
					export.c \
					unset.c \
					env.c \
					exit.c

# Source files including paths
SRCS = $(addprefix $(MAIN_DIR), $(MAIN_FILES)) \
		$(addprefix $(PARSER_DIR), $(PARSER_FILES)) \
		$(addprefix $(STRINGS_DIR), $(STRINGS_FILES)) \
		$(addprefix $(EXEC_DIR), $(EXEC_FILES)) \
		$(addprefix $(EXEC_DIR), $(addprefix $(BUILTINS_DIR), $(BUILTINS_FILES)))

# Object files including paths
# OBJS = $(SRCS:.c=.o)
SRC_FILES = $(MAIN_FILES) $(PARSER_FILES) $(STRINGS_FILES) $(EXEC_FILES) $(BUILTINS_FILES)
OBJS = $(addprefix $(OBJS_DIR), $(SRC_FILES:.c=.o))

# OBJ_FILES = $(SRCS:.c=.o)
# OBJS = $(patsubst %, $(OBJS_DIR)%, $(SRC_FILES:.c=.o))

LIBFT = $(addprefix $(LIBFT_DIR), libft.a)

CFLAGS = -Wall -Wextra -Werror -g

# Color scheme
COLOR_GREEN = \033[0;32m
COLOR_Magenta = \033[0;35m
COLOR_CYAN = \033[0;36m
COLOR_END = \033[0m

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS) $(LIBFT)
	@cc -o $(NAME) $(OBJS) $(LIBFT) -L$(RL_DIR) -lreadline -L$(LIBFT_DIR) -lft
	@echo "$(COLOR_GREEN)$(NAME) created $(COLOR_END)"

$(OBJS_DIR):
	@mkdir $(OBJS_DIR)
	@echo "$(COLOR_CYAN)object directory created$(COLOR_END)"

$(OBJS_DIR)%.o: $(MAIN_DIR)%.c
	@cc -c $< -o $@ -I$(RL_HEADER_DIR)
	@#cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(PARSER_DIR)%.c
	@cc -c $< -o $@ -I$(RL_HEADER_DIR)
	@#cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(STRINGS_DIR)%.c
	@cc -c $< -o $@ -I$(RL_HEADER_DIR)
	@#cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(EXEC_DIR)%.c
	@cc -c $< -o $@ -I$(RL_HEADER_DIR)
	@#cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(EXEC_DIR)$(BUILTINS_DIR)%.c
	@cc -c $< -o $@ -I$(RL_HEADER_DIR)
	@#cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "$(COLOR_CYAN)$(LIBFT) created$(COLOR_END)"

clean:
	@rm -fr $(OBJS_DIR)
	@echo "$(COLOR_Magenta)$(OBJS_DIR) directory and object files removed$(COLOR_END)"
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(COLOR_Magenta)$(LIBFT) and libft object files removed $(COLOR_END)"
 
fclean: clean
	@rm -f $(NAME)
	@echo "$(COLOR_Magenta)$(NAME) removed $(COLOR_END)"

re: fclean all
