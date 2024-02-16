//#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
//#include <linux/limits.h>
#include "libft/libft.h"
#include "minishell.h"

#define WHITESPACE " \t\r\n\v"
//#define SYMBOLS  "<>|&()\"\'"
#define SYMBOLS  "<>|&;()"
//#ifndef ARG_MAX
//# define ARG_MAX 100
//#endif
// for testing pupose let us use ARG_MAX = 3 at first
#define ARG_MAX 4


// Parsed command representation
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5
#define AND_CMD 6
#define OR_CMD 7


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
}	t_token_type;

// #define MAXARGS 10 redifined in ARG_MAX
/*
typedef struct s_cmd 
{
  int type;
} t_cmd;

typedef struct s_execcmd
{
  int type;
  char *argv[ARG_MAX];
  char *eargv[ARG_MAX];
}	t_execcmd;

typedef struct s_redircmd
{
  int type;
  t_cmd *cmd;
  char *file;
  char *efile;
  int mode;
  int fd;
}	t_redircmd;

typedef struct s_pipecmd
{
  int type;
  t_cmd *left;
  t_cmd *right;
}	t_pipecmd;

typedef struct s_listcmd
{
  int type;
  t_cmd *left;
  t_cmd *right;
} t_listcmd;

typedef struct s_backcmd
{
  int type;
  t_cmd *cmd;
}	t_backcmd;

*/


int fork1_test(void);  // Fork but panics on failure.
void panic_test(char*);
t_cmd *parsecmd(char*);


// Execute s_cmd.  Never returns. 
// test version for testing AST
// 

void runcmd_test(t_cmd *cmd)
{
//   int p[2];
  t_backcmd *bcmd;
  t_execcmd *ecmd;
  t_listcmd *lcmd;
  t_pipecmd *pcmd;
  t_redircmd *rcmd;

	if(cmd == 0)
	{
		printf("NULL Node\n");
    exit(1);
	}
	else if (cmd->type == EXEC)
	{
    ecmd = (t_execcmd*)cmd;
    if(ecmd->argv[0] == 0)
      exit (1);
		printf("argv=%s, %s, %s, %s\n", ecmd->argv[0], ecmd->argv[1], ecmd->argv[2], ecmd->argv[3]);
	//    exec(ecmd->argv[0], ecmd->argv);
   // printf(2, "exec %s failed\n", ecmd->argv[0]);
	}
	else if (cmd->type == REDIR)
	{
    rcmd = (t_redircmd*)cmd;
    printf("close(fd=%d);\n",rcmd->fd);
    printf("open(%s, mode=%d);\n", rcmd->file, rcmd->mode);
		/*
    if (open(rcmd->file, rcmd->mode) < 0)
		{
      printf(2, "open %s failed\n", rcmd->file);
      exit (1);
    }
		*/
    runcmd_test(rcmd->cmd);
	}
	else if (cmd->type == LIST)
	{
    lcmd = (t_listcmd*)cmd;
    if(fork1_test() == 0)
      runcmd_test(lcmd->left);
    wait(NULL);
    runcmd_test(lcmd->right);
	}
	else if (cmd->type == PIPE)
	{
    pcmd = (t_pipecmd*)cmd;
  //  if (pipe(p) < 0)
    //  panic_test("pipe");
		printf("make pipe\n");
    if (fork1_test() == 0){
//      close(1);
  //    dup(p[1]);
    //  close(p[0]);
    //  close(p[1]);
      runcmd_test(pcmd->left);
    }
  	wait(NULL);
    if (fork1_test() == 0){
//      close(0);
  //    dup(p[0]);
    //  close(p[0]);
      //close(p[1]);
      runcmd_test(pcmd->right);
    }
//    close(p[0]);
  //  close(p[1]);
    wait(NULL);
  }
	else if (cmd->type == BACK)
	{
    bcmd = (t_backcmd*)cmd;
    if(fork1_test() == 0)
      runcmd_test(bcmd->cmd);
		wait(NULL);
  }
	else
		panic_test("runcmd");
  exit (0);
}

/*
int	getcmd(char *buf, int nbuf)
{
	printf(2, "$ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
main(void)
{
  static char buf[100];
  int fd;

  // Ensure that three file descriptors are open.
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Chdir must be called by the parent, not the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0)
        printf(2, "cannot cd %s\n", buf+3);
      continue;
    }
    if(fork1_test() == 0)
      runcmd(parsecmd(buf));
    wait();
  }
  exit();
}

*/

void	panic_test(char *s)
{
//  printf(2, "%s\n", s);
	ft_putstr_fd(s, 2);
	ft_putstr_fd("\n", 2);
  exit(1);
}

int fork1_test(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic_test("fork");
  return pid;
}

//PAGEBREAK!
// Constructors

t_cmd	*execcmd(void)
{
  t_execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return ((t_cmd*)cmd);
}

t_cmd* redircmd(t_cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  t_redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  return ((t_cmd*)cmd);
}

t_cmd* pipecmd(t_cmd *left, t_cmd *right)
{
  t_pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (t_cmd*)cmd;
}

t_cmd	*listcmd(t_cmd *left, t_cmd *right)
{
  t_listcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = LIST;
  cmd->left = left;
  cmd->right = right;
  return ((t_cmd*)cmd);
}

t_cmd	*backcmd(t_cmd *subcmd)
{
  t_backcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = BACK;
  cmd->cmd = subcmd;
  return ((t_cmd*)cmd);
}

//PAGEBREAK!
// Parsing

int	gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;

  s = *ps;
  while (s < es && ft_strchr(WHITESPACE, *s))
    s++;
	
  if(q)
    *q = s;
  ret = *s;
	if (*s == 0)
		;
	else if (ft_strchr(SYMBOLS, *s))
	{
		if (s[0] == s[1] && s[0] == '>')
		{
			s++;
			ret = '+';
		}
		s++;
	}
	else
	{
		ret = 'a';
    while(s < es && !ft_strchr(WHITESPACE, *s) && !ft_strchr(SYMBOLS, *s))
      s++;
	}
  if (eq)
    *eq = s;
  while (s < es && ft_strchr(WHITESPACE, *s))
    s++;
  *ps = s;
  return (ret);
}


int	peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && ft_strchr(WHITESPACE, *s))
    s++;
  *ps = s;
  return (*s && ft_strchr(toks, *s));
}

t_cmd	*parseline(char**, char*);
t_cmd	*parsepipe(char**, char*);
t_cmd	*parseexec(char**, char*);
t_cmd	*nulterminate(t_cmd*);

t_cmd	*parsecmd(char *s)
{
  char		*es;
  t_cmd	*cmd;

  es = s + ft_strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if(s != es)
	{
    ft_putstr_fd("leftovers", 2);
		ft_putstr_fd(s, 2);
		ft_putstr_fd("\n", 2);
    panic_test("syntax");
  }
  nulterminate(cmd);
  return (cmd);
}

t_cmd	*parseline(char **ps, char *es)
{
  t_cmd *cmd;

  cmd = parsepipe(ps, es);
  while (peek(ps, es, "&"))
	{
    gettoken(ps, es, 0, 0);
    cmd = backcmd(cmd);
  }
  if(peek(ps, es, ";"))
	{
    gettoken(ps, es, 0, 0);
    cmd = listcmd(cmd, parseline(ps, es));
  }
  return (cmd);
}

t_cmd*	parsepipe(char **ps, char *es)
{
  t_cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, "|"))
	{
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return (cmd);
}

t_cmd*	parseredirs(t_cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while (peek(ps, es, "<>"))
	{
    tok = gettoken(ps, es, 0, 0);
    if (gettoken(ps, es, &q, &eq) != 'a')
      panic_test("missing file for redirection");
		if (tok == '<')
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
		else if (tok == '>')
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == '+') 
      cmd = redircmd(cmd, q, eq, O_WRONLY| O_CREAT | O_APPEND, 1);
		// else toc == "-"
		// do heredoc
  }
  return (cmd);
}

t_cmd	*parseblock(char **ps, char *es)
{
  t_cmd *cmd;

  if (!peek(ps, es, "("))
    panic_test("parseblock");
  gettoken(ps, es, 0, 0);
  cmd = parseline(ps, es);
  if (!peek(ps, es, ")"))
    panic_test("syntax - missing )");
  gettoken(ps, es, 0, 0);
  cmd = parseredirs(cmd, ps, es);
  return (cmd);
}

void	attach_to_node(t_cmd *node, t_cmd *new)
{
	t_redircmd *rnode;
	
	rnode = (t_redircmd *)node;
	rnode->cmd = new;
}

t_cmd*	parseexec(char **ps, char *es)
{
  char	*q;
	char	*eq;
  int		tok;
	int		argc;
  t_execcmd *cmd;
  t_cmd *ret;
  t_cmd *last_node;
  t_cmd *temp;

	if (peek(ps, es, "("))
		return (parseblock(ps, es));
	ret = execcmd();
	cmd = (t_execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
	last_node = ret;
  while(!peek(ps, es, "|)&;"))
	{
    tok = gettoken(ps, es, &q, &eq);
    if (tok == 0)
      break;
    if (tok != 'a')
      panic_test("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if (argc >= ARG_MAX)
      panic_test("too many args");
    temp = parseredirs((t_cmd *)cmd, ps, es);
		if (temp != (t_cmd *)cmd)
		{
			// attach temp to last_node
			if (last_node != (t_cmd *)cmd)
				attach_to_node(last_node, temp);
			else
				ret = temp;
			last_node = temp;
		}
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return (ret);
}


/*
//this is the version with reverse version of redirection nodes
t_cmd*	parseexec(char **ps, char *es)
{
  char	*q;
	char	*eq;
  int		tok;
	int		argc;
  t_execcmd *cmd;
  t_cmd *ret;

  if (peek(ps, es, "("))
		return (parseblock(ps, es));
	ret = execcmd();
  cmd = (t_execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;"))
	{
    tok = gettoken(ps, es, &q, &eq);
    if (tok == 0)
      break;
    if (tok != 'a')
      panic_test("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if (argc >= ARG_MAX)
      panic_test("too many args");
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return (ret);
}
	*/

// NUL-terminate all the counted strings.
t_cmd	*nulterminate(t_cmd *cmd)
{
  int i;
  t_backcmd *bcmd;
  t_execcmd *ecmd;
  t_listcmd *lcmd;
  t_pipecmd *pcmd;
  t_redircmd *rcmd;

	if(cmd == 0)
    return (NULL);
	else if (cmd->type == EXEC)
	{
    ecmd = (t_execcmd*)cmd;
		i = 0;
		while (ecmd->argv[i])
      *ecmd->eargv[i++] = 0;
    //for(i=0; ecmd->argv[i]; i++)
      //*ecmd->eargv[i] = 0;
	}
	else if (cmd->type == REDIR)
	{
    rcmd = (t_redircmd*)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
    pcmd = (t_pipecmd*)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
	}
	else if (cmd->type == LIST)
	{
    lcmd = (t_listcmd*)cmd;
    nulterminate(lcmd->left);
    nulterminate(lcmd->right);
	}
	else if (cmd->type == BACK)
	{
    bcmd = (t_backcmd*)cmd;
    nulterminate(bcmd->cmd);
  }
  return (cmd);
}
/* void	print_error(char *err_msg)
{
	if (ft_putstr_fd("minishell: ", STDERR_FILENO) == -1)
	{
		perror("minishell: write error");
		exit(EXIT_FAILURE);
	}
	perror(err_msg);
	exit(EXIT_FAILURE);
}

void	validate_args(int argc)
{
	if (argc != 1)
	{
		if (ft_putendl_fd(ERR_ARGS, STDERR_FILENO) == -1)
			print_error(ERR_WRITE);
		if (ft_putendl_fd(USAGE, STDOUT_FILENO) == -1)
			print_error(ERR_WRITE);
		exit(EXIT_FAILURE);
	}
}

int	arr_len(char **arr)
{
	int	len;

	len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}

void	free_arr(char **arr, int curr_index)
{
	int	i;

	i = -1;
	while (++i <= curr_index)
		free(arr[i]);
	free(arr);
	arr = NULL;
}

char	**copy_env(char **envp)
{
	int		len;
	char	**envs;
	int		i;
	int		j;

	len = arr_len(envp);
	envs = (char **)malloc(sizeof(char *) * (len + 1));
	if (envs == NULL)
		print_error(ERR_MALLOC);
	i = -1;
	while (++i < len)
	{
		envs[i] = (char *)malloc(sizeof(char) * ((int)ft_strlen(envp[i]) + 1));
		if (envs[i] == NULL)
		{
			free_arr(envs, i);
			print_error(ERR_MALLOC);
		}
		j = -1;
		while (++j < (int)ft_strlen(envp[i]))
			envs[i][j] = envp[i][j];
		envs[i][j] = '\0';
	}
	envs[i] = NULL;
	return (envs);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	data.envp = copy_env(envp); // free data.envs before exit
	//runcmd_test(parsecmd(argv[1]));
	runcmd(parsecmd(argv[1]), &data);
	
	return (0);
}
 */