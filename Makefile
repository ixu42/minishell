# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/09 11:00:23 by ixu               #+#    #+#              #
#    Updated: 2024/03/24 12:02:37 by apimikov         ###   ########.fr        #
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
				signal_utils.c \
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
				parse_heredoc_utils.c \
				parsing_utils.c \
				nulterminate.c \
				gettoken_peek.c
				# ../remove_before_submission/runcmd_test.c
STRINGS_FILES = make_argv_and_filename.c \
				expand_var.c \
				join_args.c \
				ft_free_char.c \
				strlist_join.c \
				do_single_match.c \
				match_to_files.c \
				wildcard_star.c \
				arraylist.c \
				heapsort_str.c
EXEC_FILES = runcmd.c \
				run_exec.c \
				run_redir.c \
				run_pipe.c \
				get_cmd_path.c \
				environment.c \
				exec_error.c \
				exec_error_utils.c
BUILTINS_FILES = builtins.c \
					exec_echo.c \
					exec_cd.c \
					exec_pwd.c \
					exec_export.c \
					exec_export_utils.c \
					exec_unset.c \
					exec_env.c \
					exec_exit.c \
					env.c				

# Source files including paths
SRCS = $(addprefix $(MAIN_DIR), $(MAIN_FILES)) \
		$(addprefix $(PARSER_DIR), $(PARSER_FILES)) \
		$(addprefix $(STRINGS_DIR), $(STRINGS_FILES)) \
		$(addprefix $(EXEC_DIR), $(EXEC_FILES)) \
		$(addprefix $(EXEC_DIR), $(addprefix $(BUILTINS_DIR), $(BUILTINS_FILES)))

# Object files including paths
SRC_FILES = $(MAIN_FILES) $(PARSER_FILES) $(STRINGS_FILES) $(EXEC_FILES) $(BUILTINS_FILES)
OBJS = $(addprefix $(OBJS_DIR), $(SRC_FILES:.c=.o))

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
	@cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(PARSER_DIR)%.c
	@cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(STRINGS_DIR)%.c
	@cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(EXEC_DIR)%.c
	@cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
	@echo "$(COLOR_CYAN)$@ created$(COLOR_END)"

$(OBJS_DIR)%.o: $(EXEC_DIR)$(BUILTINS_DIR)%.c
	@cc $(CFLAGS) -c $< -o $@ -I$(RL_HEADER_DIR)
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
