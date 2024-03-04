/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 06:12:04 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/04 06:12:06 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	fork1_test(void)	// Fork but panics on failure.
{
	int pid;

	pid = fork();
	if(pid == -1)
		panic_test("fork");
	return pid;
}

void    ft_print_char2d(char **split)
{
    size_t  i;

    i = 0;
    while (split[i])
        ft_dprintf(2, "\t->%s<-\n", split[i++]);
}

void	printstr(t_strcmd *str)
{
	t_strcmd	*first;
	char *start;
	char *end;
	char *joined;
	int i;

	i = 0;
	first = str;
	while (str !=NULL)
	{
		start = str->start;
		end = str->end;
		ft_dprintf(2, "   |   |---->");
		write(2, start, end - start);
		ft_dprintf(2, "<-\t type=%d, flag=%d\n", str->type, str->flag);
		str = str->next;
		i++;
	}
	joined = strlist_join(first);
	if (joined)
	{
		ft_dprintf(2, "   |     joined string  ->%s<-\n", joined);
		free(joined);
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
		ft_dprintf(2,"   |--arg-%d\t->", i);
		if (start < end)
			write(2, start, end - start);
		ft_dprintf(2, "<-\n");
		printstr(args->left);
		ft_dprintf(2, "   |\n");
		if (args->right == args)
			break;
		args = args->right;
		i++;
	}
	if (args == NULL)
		ft_dprintf(2, "   NULL terminated arg node.\n");
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
		printf("Error: NULL Node in AST\n");
		//exit(1);
	}
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd*)cmd;
		make_argv(ecmd, NULL);
		if(ecmd->argv[0] == 0)
		{
			ft_dprintf(2, "runcmd_test: EXEC argv is empty\n");
//			exit (1);
		}
		ft_dprintf(2, "EXEC:    MAXARG limited sargv=%s, %s, %s, %s\n", ecmd->sargv[0], ecmd->sargv[1], ecmd->sargv[2], ecmd->sargv[3]);
		ft_dprintf(2, "    argv=\n");
		ft_print_char2d(ecmd->argv);
		printargs(ecmd->args);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd*)cmd;
		ft_dprintf(2, "REDIR: file=%s, mode=%d, fd=%d);\n", rcmd->file, rcmd->mode,rcmd->fd);
		printstr(rcmd->str);
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
		printf("make pipe\n");
		if (fork1_test() == 0){
			runcmd_test(pcmd->left);
		}
		wait(NULL);
		if (fork1_test() == 0){
			runcmd_test(pcmd->right);
		}
		wait(NULL);
	}
	else
		ft_dprintf(2, "Error: undefined node of AST\n");
}
