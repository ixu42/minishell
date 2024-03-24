/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runcmd_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 06:12:04 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 11:30:17 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void printf_nonprintable(char *str)
{
    int i;

    i = 0;
    printf("String with non-printable: ->");
    while (str[i] != '\0')
    {
        if (str[i] < 32 || str[i] > 126)
            printf("\\x%02X", (unsigned char)str[i]);
        else
            printf("%c", str[i]);
        i++;
    }
    printf("<-\n");
}

int	fork1_test(void)	// Fork but panics on failure.
{
	int	pid;

	pid = fork();
	if (pid == -1)
		ft_dprintf(2, "fork");
	return (pid);
}

void	ft_print_char2d(char **split)
{
	size_t	i;

	i = 0;
	while (split[i])
		ft_dprintf(2, "\t->%s<-\n", split[i++]);
}

/* 
char *locate_var_env(char *start, char *end, t_env *env_list)
{
//	char	*value;
  t_env	*node;

//	name_in_env_lst(t_env *env_lst, char *arg, size_t name_len, t_env **node)
  if (name_in_env_lst(env_list, start, end - start, &node))
		return (node->value);
	return (NULL);
}
*/

void	printstr(t_strcmd *str, t_data *data)
{
	t_strcmd	*first;
	char		*start;
	char		*end;
	char		*joined;
	int			i;

	i = 0;
	first = str;
	while (str != NULL)
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

void	printargs(t_argcmd *args, t_data *data)
{
	char	*start;
	char	*end;
	int		i;

	i = 0;
	while (args != NULL)
	{
		start = args->start;
		end = args->end;
		ft_dprintf(2, "   |--arg-%d\t->", i);
		if (start < end)
			write(2, start, end - start);
		ft_dprintf(2, "<- flag=%d\n",args->flag);
		printstr(args->left, data);
		ft_dprintf(2, "   |\n");
		if (args->right == args)
			break ;
		args = args->right;
		i++;
	}
	if (args == NULL)
		ft_dprintf(2, "   NULL terminated arg node.\n");
}
// test version of runcmd for testing AST
void	runcmd_test(t_cmd *cmd, t_data *data)
{
	int			p[2];
	t_execcmd	*ecmd;
	t_listcmd	*lcmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;

	if (cmd == NULL)
		printf("Error: NULL Node in AST\n");
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		make_argv(ecmd, data);
		if (ecmd->argv[0] == 0)
			ft_dprintf(2, "runcmd_test: EXEC argv is empty\n");
		ft_dprintf(2, "EXEC:   flag=%d, argc=%d\n", ecmd->flag, ecmd->argc);
		ft_dprintf(2, "    argv=\n");
		ft_print_char2d(ecmd->argv);
		printargs(ecmd->args, data);
		//ft_dprintf(2, "    MAXARG limited sargv=%s, %s, %s, %s\n", ecmd->sargv[0], ecmd->sargv[1], ecmd->sargv[2], ecmd->sargv[3]);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		make_filename(rcmd, data);
		ft_dprintf(2, "REDIR: file=%s, mode=%d, fd=%d, flag=%d);\n", \
			rcmd->file, rcmd->mode, rcmd->fd, rcmd->flag);
		printstr(rcmd->str, data);
		runcmd_test(rcmd->cmd, data);
	}
	/*
	else if (cmd->type == LIST)
	{
		lcmd = (t_listcmd *)cmd;
		if (fork1_test() == 0)
		{
			runcmd_test(lcmd->left, data);
			exit (0);
		}
		wait(NULL);
		runcmd_test(lcmd->right, data);
	}
		*/
	else if (cmd->type == AND_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		if (fork1_test() == 0)
		{
			runcmd_test(lcmd->left, data);
			exit (0);
		}
		wait(NULL);
		printf("&& flag=%d\n", lcmd->flag);
		runcmd_test(lcmd->right, data);
	}
	else if (cmd->type == OR_CMD)
	{
		lcmd = (t_listcmd *)cmd;
		if (fork1_test() == 0)
		{
			runcmd_test(lcmd->left, data);
			exit (0);
		}
		wait(NULL);
		printf("|| flag=%d\n", lcmd->flag);
		runcmd_test(lcmd->right, data);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		printf("make pipe,  flag=%d\n", cmd->flag);
		if (fork1_test() == 0){
			runcmd_test(pcmd->left, data);
			exit (0);
		}
		wait(NULL);
		if (fork1_test() == 0){
			runcmd_test(pcmd->right, data);
			exit (0);
		}
		wait (NULL);
	}
	else
		ft_dprintf(2, "Error: undefined node of AST\n");
}
