#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/include/libft.h"
# include "libft/include/ft_dprintf.h"

// readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

// write, close, dup2, fork, pipe, access, execve, unlink
# include <unistd.h>

//FILE CONTROL
# include <fcntl.h>

// EXIT_FAILURE, EXIT_SUCCESS, NULL
# include <stdlib.h>

// wait
# include <sys/wait.h>

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
# define ERR_ID "not a valid identifier"
# define PMT "\033[0;31mLiteShell: \033[0m"
# define PMT_ERR_WRITE "\033[0;31mLiteShell: \033[0mwrite error"
# define PMT_ERR_PRINTF "\033[0;31mLiteShell: \033[0mprintf error"
# define PMT_ERR_GETCWD "\033[0;31mLiteShell: \033[0mgetcwd error"
# define EXIT_CMD_PERM_ERR 126
# define EXIT_CMD_NOT_FOUND 127

// macros for processes
# define PARENT_PROC 0
# define CHILD_PROC 1


#define TESTMODE 1 
#define MAXARGS 4
#define WHITESPACE  " \t\r\n\v"
//#define SYMBOLS "<>|&;()\\"
#define SYMBOLS "<>|&;()"
#define ERR_SYNTAX_UNEXP "syntax error near unexpected token" 

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
	STR_STAR,
	// to be remove?
	LIST
}   t_node_type;

// types of tockens
typedef enum e_token
{
	UNDEFINED_TOK,
	STR_TOK,
	RED_IN,
	RED_OUT,
	HEREDOC,
	RED_OUT_APP,
	PIPE_TOK,
	OR_TOK,
	AND_TOK,
	//unused tok
	LPAR,
	RPAR
}   t_token_type;

typedef enum e_parse_error
{
	SYNTAX_ERR_UNDEFTOK = 0x1,
	SYNTAX_ERROR = 0x2,
	MALLOC_ERROR = 0x4,
}	t_parse_error;

typedef enum e_builtin
{
	ECHO = 1,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
}   t_builtin;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
	char		*buf;
	char		**envp;
	t_env		*env_lst;
	char		**env_paths;
	char		*cmd_path;
	t_builtin	builtin;
}	t_data;

typedef struct s_strstate
{
	char	*start;
	char	*pos;
	char	*finish;
	char	*beg;
	char	*end;
	int		d_quotes;
	int		s_quotes;
	int		flag;
} t_strstate;

typedef struct s_cmd
{
	int	type;
}	t_cmd;

typedef struct s_strcmd
{
	int		type;
	int		flag;
	char	*start;
	char	*end;
	struct s_strcmd	*next;
}	t_strcmd;

typedef struct s_argcmd
{
	int		type;
	int		flag;
	t_strcmd	*left;
	struct s_argcmd	*right;
	char			*start;
	char			*end;
}	t_argcmd;

typedef struct s_execcmd
{
	int		type;
	char	*sargv[MAXARGS];
	char	*eargv[MAXARGS];
	char	**argv;
	int		argc;
	t_argcmd	*args;
}	t_execcmd;

typedef struct s_redircmd
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
	t_strcmd	*str;
	int		flag;
}	t_redircmd;

typedef struct s_pipecmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}	t_pipecmd;

typedef struct s_listcmd
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}	t_listcmd;

int		fork1(t_data *data);
t_cmd	*parsecmd(char *s);
int		runcmd(t_cmd *cmd, t_data *data, int child_proc);
// to be removed at some point
void	runcmd_old(t_cmd *cmd, t_data *data);
void	runcmd_test(t_cmd *cmd);

// constructors.c
t_cmd		*execcmd(void);
t_cmd		*redircmd_old(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd		*redircmd(t_cmd *subcmd, t_strstate *state, int mode, int fd);
t_cmd		*pipecmd(t_cmd *left, t_cmd *right);
t_cmd		*list_cmd(t_cmd *left, t_cmd *right, int type);
t_argcmd	*argcmd(t_strcmd *str, t_argcmd *args, char *start, char *end);
t_strcmd	*strcmd(int type, char *start, char *end);
t_strstate	*make_strstate(char *pos, char *finish);

// parseexec.c
t_cmd	*parseexec(char**, char*);
t_cmd	*parseredirs(t_cmd *cmd, char **ps, char *es);

// parsecmd.c
t_cmd   *parsecmd(char*);
t_cmd   *parseblock(char **ps, char *es);
//t_cmd   *parseline(char**, char*);
//t_cmd   *parsepipe(char**, char*);

//parsing_utils.c
int gettoken(char **ps, char *es, char **q, char **eq);
int peek(char **ps, char *es, char *toks);
t_cmd   *nulterminate(t_cmd *cmd);
void    panic_test(char *s);  //this is temporal function that exit(1) from parser

// data init
t_env	*copy_env_arr_to_lst(char **envp);
char	**get_env_paths(char **envp, t_data *data);
void	data_init(t_data *data, char **envp);

// data init utils
char	*get_value(char *name_value_str);
t_env	*get_node(char *name_value_str);
void	lst_append(t_env **env_lst, t_env *new_node);
void	print_error_partial_free(char *name, t_data *data);

// handling builtins
int		exec_echo(char **argv);
int		exec_exit(char **argv);
int		exec_cd(char **argv, t_env *env_lst);
int		exec_pwd(char **argv);
int		exec_export(char **argv, t_env *env_lst);
int		exec_unset(char **argv, t_env *env_lst);
int		exec_env(t_env *env_lst);
int		name_in_env_lst(t_env *env_lst, char *arg, size_t name_len, t_env **node);
int		is_builtin(char **argv, t_data **data);
int		run_builtin(char **argv, t_data *data);

// handling env
char	**copy_env(char **envp);
char	*get_value(char *name_value_str);
t_env	*get_node(char *name_value_str);
void	lst_append(t_env **env_lst, t_env *new_node);
t_env	*copy_env_arr_to_lst(char **envp);
char	**copy_env_lst_to_arr(t_env *env_lst);
char	**get_env_paths(char **envp, t_data *data);

// parsing cmd path
char	*get_cmd_path(char **argv, t_data *data);

// freeing
void	free_arr(char **arr);
void	free_data(t_data *data);

// error handling
void	validate_args(int argc);
void	print_error_n_exit(char *err_msg);
void	panic(char *err_msg, t_data *data, int exit_code);

// readline
void	rl_clear_history(void);

// string operations
char	*strlist_join(t_strcmd *str);
int		make_argv(t_execcmd *cmd, t_data *data);

#endif
