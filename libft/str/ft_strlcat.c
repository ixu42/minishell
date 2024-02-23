/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:36:14 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:58:07 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	i;

	src_len = ft_strlen(src);
	if (!dst && !dstsize)
		return (dstsize + src_len);
	dst_len = ft_strlen(dst);
	i = 0;
	if (dstsize < dst_len + 1 || dstsize == 0)
		return (dstsize + src_len);
	else
	{
		while (i < src_len && i < (dstsize - dst_len - 1))
		{
			dst[dst_len + i] = src[i];
			i++;
		}
		dst[dst_len + i] = '\0';
	}
	return (dst_len + src_len);
}
