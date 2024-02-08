/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:37:05 by ixu               #+#    #+#             */
/*   Updated: 2024/02/09 11:09:14 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

// readline
# include <stdio.h>
// # include <readline/readline.h>
// # include <readline/history.h>

// STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO
# include <unistd.h>

// EXIT_FAILURE, EXIT_SUCCESS
# include <stdlib.h>

// macros for (error) messages
# define ERR_WRITE "write error"
# define ERR_ARGS "invalid number of arguments"
# define USAGE "ex. usage: ./minishell"

#endif
