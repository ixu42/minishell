/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:54:06 by ixu               #+#    #+#             */
/*   Updated: 2023/11/06 15:37:48 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;
	int		result;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		result = (int)((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (result != 0)
			return (result);
		i++;
	}
	return (0);
}
