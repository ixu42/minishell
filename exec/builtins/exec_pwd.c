/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 17:07:53 by ixu               #+#    #+#             */
/*   Updated: 2024/03/23 13:53:53 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	exec_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror(PMT_ERR_GETCWD);
		return (1);
	}
	if (printf("%s\n", cwd) < 0)
	{
		perror(PMT_ERR_PRINTF);
		return (1);
	}
	return (0);
}
