/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 21:14:29 by ixu               #+#    #+#             */
/*   Updated: 2024/03/24 18:40:59 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/include/libft.h"
# include "libft/include/ft_dprintf.h"
# include "libft/include/get_next_line.h"

// readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

// write, close, dup, dup2, fork, pipe, access, execve, unlink
# include <unistd.h>

// FILE CONTROL
# include <fcntl.h>

// stat
# include <sys/stat.h>

// NULL, malloc, free
# include <stdlib.h>

// waitpid
# include <sys/wait.h>

// sigaction
# include <signal.h>

// tcgetattr, tcsetattr
# include <termios.h>

//directory  entity
# include <dirent.h>

// macros for (error) messages
# define ERR_ARGS "invalid number of arguments"
# define USAGE "ex. usage: ./minishell"
# define ERR_WRITE "write error"
# define ERR_PRINTF "printf error"
# define ERR_MALLOC "malloc error"
# define ERR_PIPE "pipe error"
# define ERR_FORK "fork error"
# define ERR_WAITPID "waitpid error"
# define ERR_OPEN "open error"
# define ERR_CLOSE "close error"
# define ERR_DUP2 "dup2 error"
# define ERR_DUP "dup error"
# define ERR_HEREDOC "heredoc error"
# define ERR_SIGACTION "sigaction error"
# define ERR_ID "not a valid identifier"
# define PMT "\033[0;31mLiteShell: \033[0m"
# define PMT_ERR_WRITE "\033[0;31mLiteShell: \033[0mwrite error"
# define PMT_ERR_PRINTF "\033[0;31mLiteShell: \033[0mprintf error"
# define PMT_ERR_GETCWD "\033[0;31mLiteShell: \033[0mgetcwd error"
# define PMT_ERR_MALLOC "\033[0;31mLiteShell: \033[0mmalloc error"

// macros for processes
# define PARENT_PROC 0
# define CHILD_PROC 1

// macros for 2d arraylist
# define INITIAL_CAPACITY 5
# define GROWTH_FACTOR 2

// macros for parsing 
# define TESTMODE 0
# define MAXARGS 4
# define WHITESPACE  " \t\r\n\v"
# define SYMBOLS "<>|&()"
# define ASCII_EMPTY_X "\x1d"
# define ASCII_EMPTY 29
# define ASCII_WILD 30
# define ASCII_SEPARATOR 31
# define ERR_SYNTAX_UNEXP "syntax error near unexpected token" 
# define ERR_REDIR_AMBIG  "ambiguous redirect"
# define ERR_CODE_SYNTAX 258

// macros for termios
# define SET_ECHOCTL 1
# define UNSET_ECHOCTL 0

extern volatile sig_atomic_t	g_last_sig;

// AST's node types
typedef enum e_node_type
{
	EXEC = 1,
	REDIR,
	PIPE,
	AND_CMD,
	OR_CMD,
	ARG_NODE,
	STR_NODE,
	STR_NODE_VAR,
	STR_NODE_VAR_P,
	STR_EXIT_CODE,
	STR_STAR
}	t_node_type;

// types of tockens
typedef enum e_token
{
	UNDEFINED_TOK,
	NEWLINE_TOK,
	STR_TOK,
	RED_IN,
	RED_OUT,
	HEREDOC,
	RED_OUT_APP,
	PIPE_TOK,
	OR_TOK,
	AND_TOK,
	LPAR,
	RPAR
}	t_token_type;

typedef enum e_parse_error
{
	SYNTAX_ERR_UNDEFTOK = 0x01,
	SYNTAX_ERR_UNEXPTOK = 0x02,
	SYNTAX_ERR_UNCLOSED = 0x04,
	SYNTAX_ERROR = 0x08,
	MALLOC_ERROR = 0x10,
	DIR_OPEN_ERR = 0x20,
	HEREDOC_OPEN_ERR = 0x40,
	SIGNAL_CTRL_C = 0x80,
	ENOMEM_ERR = 0x100
}	t_parse_error;

typedef enum e_wild_error
{
	WILD_ERR_REDIR_AMBIG = 0x01,
	WILD_ERR_DIR = 0x02,
	WILD_MALLOC_ERROR = 0x10,
	WILD_ERR_TERMINATE = 0x20
}	t_wild_error;

typedef enum e_builtin
{
	ECHO_CMD = 1,
	CD_CMD,
	PWD_CMD,
	EXPORT_CMD,
	UNSET_CMD,
	ENV_CMD,
	EXIT_CMD,
}	t_builtin;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	int	type;
	int	flag;
}	t_cmd;

typedef struct s_data
{
	char		*buf;
	char		**envp;
	t_env		*env_lst;
	char		**env_paths;
	char		*cmd_path;
	t_builtin	builtin;
	int			proc;
	int			status;
	char		*stat_str;
	int			fd_stdin;
	int			fd_stdout;
	int			under_pipe;
	int			under_redir;
	t_cmd		*tree;
}	t_data;

typedef struct s_arrlist
{
	char	**data;
	size_t	size;
	size_t	capacity;
}	t_arrlist;

typedef struct s_wildcard
{
	t_arrlist	*list;
	t_arrlist	**pnt;
	char		**argv;
	int			argc;
	DIR			*directory;
}	t_wildcard;

typedef struct s_strstate
{
	char	*start;
	char	*pos;
	char	*finish;
	char	*beg;
	char	*end;
	int		d_quotes;
	//int		s_quotes;
	int		flag;
	char	*heredoc;
}	t_strstate;

typedef struct s_aststate
{
	char	*start;
	char	*ps;
	char	*es;
	int		flag;
	int		heredoc;
}	t_aststate;


typedef struct s_strcmd
{
	int				type;
	int				flag;
	char			*start;
	char			*end;
	struct s_strcmd	*next;
	int				malloc_flag;
}	t_strcmd;

typedef struct s_argcmd
{
	int				type;
	int				flag;
	t_strcmd		*left;
	struct s_argcmd	*right;
	char			*start;
	char			*end;
}	t_argcmd;

typedef struct s_execcmd
{
	int			type;
	int			flag;
	char		*sargv[MAXARGS];
	char		*eargv[MAXARGS];
	char		**argv;
	int			argc;
	t_argcmd	*args;
	t_arrlist	*list;
}	t_execcmd;

typedef struct s_redircmd
{
	int			type;
	int			flag;
	t_cmd		*cmd;
	char		*file;
	char		*sfile;
	char		*efile;
	int			mode;
	int			fd;
	char		*heredoc;
	t_strcmd	*str;
	t_arrlist	*list;
}	t_redircmd;

typedef struct s_pipecmd
{
	int		type;
	int		flag;
	t_cmd	*left;
	t_cmd	*right;
}	t_pipecmd;

typedef struct s_listcmd
{
	int		type;
	int		flag;
	t_cmd	*left;
	t_cmd	*right;
}	t_listcmd;

// readline
void		rl_clear_history(void);
void		rl_replace_line(const char *text, int clear_undo);

// data init
t_env		*copy_env_arr_to_lst(char **envp);
char		**get_env_paths(char **envp, t_data *data);
void		data_init(t_data *data, char **envp);

// data init utils
t_env		*get_node_in_init(char *name_value_str);
void		lst_append_in_init(t_env **env_lst, t_env *new_node);
void		print_error_partial_free(char *name, t_data *data);

// signal handling
int			set_signals_interactive(void);
int			set_default_signals(void);
int			ignore_signals(void);
int			heredoc_signal_handler(void);
void		display_pmt_on_nl(int signum);
void		move_to_nl(int signum);

// constructors_tree.c
t_cmd		*execcmd(void);
t_cmd		*redircmd(t_cmd *subcmd, t_strstate *state, int mode, int fd);
t_cmd		*pipecmd(t_cmd *left, t_cmd *right);
t_cmd		*list_cmd(t_cmd *left, t_cmd *right, int type);

// constructors_aux.c
t_argcmd	*argcmd(t_strcmd *str, t_argcmd *args, char *start, char *end);
t_strcmd	*strcmd(int type, char *start, char *end);
t_strstate	*make_strstate(char *pos, char *finish);
t_aststate	*make_aststate(char *pos, char *finish);


t_strcmd	*parsestr(t_strstate *state);
// parseexec.c
t_cmd		*parseexec(char **ps, char *es, t_aststate *ast);
t_cmd		*parseredirs(t_cmd *cmd, char **ps, char *es, t_aststate *ast);
int			get_input_heredoc(t_strstate *state, t_aststate *ast, char *delimiter);

// make_ask.c
int			make_ast(t_cmd **p_cmd, char *s);
t_cmd   *parsecmd(char *buf, int *status);

// parseline.c
t_cmd   *parseblock(char **ps, char *es, t_aststate *ast);
t_cmd	*parseline(char **ps, char *es, t_aststate *ast);

//parseredirs.c
t_cmd	*combine_redirs(t_cmd *head, t_cmd *extra, t_cmd *cmd);

//parse_word_singl_var.c
t_strcmd	*parse_word(t_strstate *state);
t_strcmd	*parse_str_till(t_strstate *state, char *stop_toks);
t_strcmd	*parse_single(t_strstate *state);
t_strcmd	*parse_variable(t_strstate *state);

// free_exec.c
int	free_str(t_strcmd *cmd);
int	free_exec(t_cmd *cmd);

// freecmd.c
int	freecmd(t_cmd *cmd);
void	freecmd_null(t_cmd **cmd);

//parsing_utils.c
void    increase_s_quotes(char **pnt_s, int *p_quotes);
t_cmd   *nulterminate(t_cmd *cmd);
int	panic_parser(char *s, int err);
int select_token(char **pnt);

//gettoken_peek.c
int gettoken(char **ps, char *es, char **q, char **eq);
int peek(char **ps, char *es, char *toks);
const char  *token_type_to_str(t_token_type token);

// runcmd() func and its helper funcs
void		runcmd(t_cmd *cmd, t_data *data);
void		run_exec(t_cmd *cmd, t_data *data);
int			run_redir(t_cmd *cmd, t_data *data);
void		run_and(t_cmd *cmd, t_data *data);
void		run_or(t_cmd *cmd, t_data *data);
int			run_pipe(t_cmd *cmd, t_data *data);
int			fork1(t_data *data);
int			restore_stdin_n_stdout(t_data *data);
char		*get_cmd_path(char **argv, t_data *data);
char		**copy_env_lst_to_arr(t_env *env_lst, t_data *data);
// to be removed at some point
void		runcmd_test(t_cmd *cmd, t_data *data);

// handling builtins
int			is_builtin(char **argv, t_data **data);
int			run_builtin(char **argv, t_data *data);
int			exec_echo(char **argv);
int			exec_exit(char **argv, t_data *data);
int			exec_cd(char **argv, t_data *data);
int			exec_pwd(void);
int			exec_export(char **argv, t_data *data);
int			exec_unset(char **argv, t_data *data);
int			exec_env(t_env *env_lst);
int			name_in_env_lst(t_env *env_lst, char *arg, size_t name_len, t_env **node);
char		*get_value(char *name_value_str, t_env *new_node, int *err_flag);
t_env		*get_node(char *name_value_str);
void		lst_append(t_env **env_lst, t_env *new_node);

// handling env
char		**copy_env(char **envp);
t_env		*copy_env_arr_to_lst(char **envp);
char		**get_env_paths(char **envp, t_data *data);

// freeing
void		free_arr(char **arr);
void		free_data(t_data *data);
void		free_n_exit(t_data *data, int status_code);

// error handling
void		validate_args(int argc);
int			is_valid_buf(char *buf);
void		print_error_n_exit(char *err_msg);
t_env		*error_handler(char *err_msg, int *err_flag);
int			panic(char *msg, t_data *data, int status_code);
int			panic_is_a_dir(char *msg, t_data *data);
int			panic_cmd_not_found(char *cmd, t_data *data);
int			perror_n_return(char *msg, int return_value);
char		*join_msgs(char const *s1, char const *s2, char const *s3);
int			handle_exit_or_return(t_data *data, int status_code);

// string operations
// do_single_match.c
int	do_single_match(int i, t_wildcard *wild, char *str, DIR *dir);
int free_wildcard(t_wildcard *wild, int clean_list, int error);
int init_wildcard(t_wildcard *wild, t_execcmd *cmd);
int init_wildcard_redir(t_wildcard *wild, t_redircmd *cmd);
// match_to_files.c
int match_to_files(t_wildcard *wild);
int make_sorted_argv(t_wildcard *wild, int be_sorted);
int copy_sorted_argv(t_wildcard *wild);

//join_args.c
char    *join_all_arguments(char **pnt, t_execcmd *cmd);
int get_argc(t_execcmd *cmd);

// ft_free_char.c
void    ft_free_char2d(char **split);
int ft_free_char2d_return(char **split, int ret);

// expand_var.c
void    expand_var_in_strlist(t_strcmd *str, t_data *data);


char	*strlist_join(t_strcmd *str);
int		make_argv(t_execcmd *cmd, t_data *data);
int		make_filename(t_redircmd *rcmd, t_data *data);
int		match(const char *pattern, const char *text);
void	ft_free_char2d(char **split);
void	heapsort_str(char **arr, int n);


// arraylist
t_arrlist	*create_arrlist(void);
int			add_string_arrlist(t_arrlist *list, const char *str);
void		free_arrlist(t_arrlist *list);
int			wildcard_star(t_execcmd *cmd);
int			wildcard_star_redir(t_redircmd *cmd);

#endif
