/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 15:30:57 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:56:45 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	if (!dst && !src)
		return (NULL);
	if (dst < src)
	{
		i = 0;
		while (i < len)
		{
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	if (dst > src && len != 0)
	{
		i = len - 1;
		while (i > 0)
		{
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
			i = i - 1;
		}
		((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
	}
	return (dst);
}
