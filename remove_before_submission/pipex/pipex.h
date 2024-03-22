/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:48:28 by ixu               #+#    #+#             */
/*   Updated: 2024/01/03 17:46:09 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
// malloc, free, exit
# include <stdlib.h>
/* write, close, dup2, fork, pipe, access, execve, unlink
STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO */
# include <unistd.h>
// open
# include <fcntl.h>
// fork
# include <sys/types.h>
// waitpid
# include <sys/wait.h>
// perror
# include <stdio.h>

# define EX_ARGS "e.g.: ./pipex infile cmd1 cmd2 outfile"
# define ERR_INPUT "Error: invalid number of arguments"
# define ERR_CMD ": command not found"
# define ERR_WRITE "write error"
# define ERR_CLOSE "close error"
# define ERR_PIPE "pipe error"
# define ERR_DUP2 "dup2 error"
# define ERR_FORK "fork error"
# define ERR_WAITPID "waitpid error"
# define ERR_MALLOC "malloc error"
# define EXIT_CMD_PERM_ERR 126
# define EXIT_CMD_NOT_FOUND 127

typedef struct s_data
{
	char	**env_paths;
	char	**cmd_arr;
	char	*cmd_path;
}	t_data;

// parsing
char	**get_cmd_arr(char *cmd_arg, t_data *data);
char	*get_cmd_path(t_data *data);
char	**get_env_paths(char **envp, t_data *data);

// utils
void	close_fds(t_data *data, int fd1, int fd2);
void	print_error(char *name, t_data *data, int exit_code);
void	print_error_partial_free(char *name, t_data *data);
void	print_error_no_free(char *name);
void	data_init(t_data **data, char **envp);

// freeing
void	free_arr(char **arr);
void	free_struct(t_data *data);

#endif