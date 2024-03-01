#include "libft/include/libft.h"
#include "minishell.h"
#include <stdio.h>

int	fork1_test(void);	// Fork but panics on failure.

/*
void	printstr(t_strcmd *str)
{
	char *start;
	char *end;
	int i;

	i = 0;
	while (str !=NULL)
	{
		start = str->start;
		end = str->end;
		ft_dprintf(2, "\t str i=%d, len=%d\n", i, (int)(end - start));
//		printf("address=%p\n", str);
//		if (start < end)
		write(2, "\t ", 2);
		write(2, start, end - start);
		printf("\n");
		str = str->next;
		i++;
	}
}

void	printargs(t_argcmd *args)
{
	char *start;
	char *end;
	int i;

	i = 0;
	while (args != NULL)
	{
		//printf("arg number %d for pnt=%p=>\n", i, args);
		//printf("arg number %d for rrr=%p=>\n", i, args->right);
		start = args->start;
		end = args->end;
		ft_dprintf(2,"argument i=%d : ", i);
		if (start < end)
			write(2, start, end - start);
		ft_dprintf(2, "\n");
		printstr(args->left);
		if (args->right == args)
			break;
		args = args->right;
		i++;
	}
	if (args == NULL)
		printf("NULL terminated arg node.");
}
// test version of runcmd for testing AST
void	runcmd_test(t_cmd *cmd)
{
	int p[2];
	t_execcmd *ecmd;
	t_listcmd *lcmd;
	t_pipecmd *pcmd;
	t_redircmd *rcmd;

	if(cmd == NULL)
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
		printargs(ecmd->args);
	//		exec(ecmd->argv[0], ecmd->argv);
	 // printf(2, "exec %s failed\n", ecmd->argv[0]);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd*)cmd;
		printf("close(fd=%d);\n",rcmd->fd);
//		printf("hi\n");
//		printf("open(%%s, mode=%d);\n", rcmd->mode);
//		printf("open(%p, mode=%%d);\n", rcmd->file);
//		write(2, rcmd->file, 10);
		printf("open(%s, mode=%d);\n", rcmd->file, rcmd->mode);
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
	else if (cmd->type == AND_CMD)
	{
		lcmd = (t_listcmd*)cmd;
		if(fork1_test() == 0)
			runcmd_test(lcmd->left);
		wait(NULL);
		printf("&&\n");
		runcmd_test(lcmd->right);
	}
	else if (cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd*)cmd;
		if(fork1_test() == 0)
			runcmd_test(lcmd->left);
		wait(NULL);
		printf("||\n");
		runcmd_test(lcmd->right);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd*)cmd;
	//	if (pipe(p) < 0)
		//	panic_test("pipe");
		printf("make pipe\n");
		if (fork1_test() == 0){
//			close(1);
	//		dup(p[1]);
		//	close(p[0]);
		//	close(p[1]);
			runcmd_test(pcmd->left);
		}
		wait(NULL);
		if (fork1_test() == 0){
//			close(0);
	//		dup(p[0]);
		//	close(p[0]);
			//close(p[1]);
			runcmd_test(pcmd->right);
		}
//		close(p[0]);
	//	close(p[1]);
		wait(NULL);
	}
	else
		panic_test("runcmd");
	exit (0);
}
*/

int fork1_test(void)
{
	int pid;

	pid = fork();
	if(pid == -1)
		panic_test("fork");
	return pid;
}

void	print_error(char *err_msg)
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
	t_cmd		*tree;

	(void)argv;
	data.envp = copy_env(envp); // free data.envs before exit
//	runcmd_old(parsecmd(argv[1]), &data);
	//printf("(=%d, |=%d, &=%d, <=%d, >=%d, +=%d\n",'(','|','&','<','>','+');
	//printf("&&=%d, ||=%d\n", AND_TOK, OR_TOK);
	tree = parsecmd(argv[1]);
	printf("parsing finnished\n");
	runcmd_test(tree);

	return (0);
}