#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/include/libft.h"
# include "libft/include/ft_dprintf.h"

// readline
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

/* write, close, dup2, fork, pipe, access, execve, unlink
STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO */
# include <unistd.h>

//FILE CONTROL
# include <fcntl.h>

// EXIT_FAILURE, EXIT_SUCCESS, NULL
# include <stdlib.h>

// wait
# include <sys/wait.h>

// macros for (error) messages
# define ERR_WRITE "write error"
# define ERR_ARGS "invalid number of arguments"
# define USAGE "ex. usage: ./minishell"

# define ERR_MALLOC "malloc error"
# define ERR_PIPE "pipe error"
# define ERR_FORK "fork error"
# define ERR_WAITPID "waitpid error"
// # define EXIT_CMD_PERM_ERR 126
# define EXIT_CMD_NOT_FOUND 127

# define PARENT_PROC 0
# define CHILD_PROC 1


#define MAXARGS 10
#define WHITESPACE  " \t\r\n\v"
#define SYMBOLS "<>|&;()\\"

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
	STR_NODE_EXT,
	// to be remove?
	LIST
}   t_node_type;

// types of tockens
typedef enum e_token
{
	WORD,
	RED_IN,
	HEREDOC,
	RED_OUT,
	RED_OUT_APP,
	PIPE_TOK,
	OR_TOK,
	AND_TOK,
	LPAR,
	RPAR
}   t_token_type;

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

typedef struct s_data
{
	char		*buf;
	char		**envp;
	t_builtin	builtin;
}	t_data;

typedef struct s_cmd
{
	int	type;
}	t_cmd;

typedef struct s_execcmd
{
	int		type;
	char	*argv[MAXARGS];
	char	*eargv[MAXARGS];
	t_cmd	*args;
}	t_execcmd;

/*
typedef struct s_argcmd
{
	int		type;
	t_cmd	*data;
	t_cmd	*next;
}	t_argcmd;
*/

typedef struct s_strcmd
{
	int		type;
	char	*start;
	char	*end;
	t_cmd	*next;
}	t_strcmd;

typedef struct s_redircmd
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	char	*efile;
	int		mode;
	int		fd;
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

char	**copy_env(char **envp);
int		fork1(t_data *data);
void	panic(char *err_msg, t_data *data, int exit_code);
t_cmd	*parsecmd(char *s);
int		is_builtin(char **argv, t_data **data);
int		run_builtin(char **argv, t_data *data);
int		runcmd(t_cmd *cmd, t_data *data, int child_proc);
// to be removed at some point
void	runcmd_old(t_cmd *cmd, t_data *data);
void	runcmd_test(t_cmd *cmd);

// constructors.c
t_cmd   *execcmd(void);
t_cmd   *strcmd(int type);
t_cmd   *redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd);
t_cmd   *pipecmd(t_cmd *left, t_cmd *right);
t_cmd   *list_cmd(t_cmd *left, t_cmd *right, int type);

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

#endif
