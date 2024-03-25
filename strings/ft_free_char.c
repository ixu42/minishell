/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:57:16 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 17:32:26 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_char2d(char **split)
{
	size_t	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

int	ft_free_char2d_return(char **split, int ret)
{
	ft_free_char2d(split);
	return (ret);
}

void	free_null(void **pnt)
{
	if (*pnt)
		free(*pnt);
	*pnt = NULL;
}
