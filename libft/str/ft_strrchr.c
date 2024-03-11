/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 19:08:12 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:58:49 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*ptr;
	char	*ref;

	ptr = (char *)s;
	ref = ptr;
	while (*s)
	{
		s++;
		ptr++;
	}
	while (s != ref)
	{
		if (*s == (char)c)
			return (ptr);
		s--;
		ptr--;
	}
	if (*s == (char)c)
		return (ptr);
	return (NULL);
}
