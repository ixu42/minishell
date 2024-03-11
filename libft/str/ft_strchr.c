/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 18:30:29 by ixu               #+#    #+#             */
/*   Updated: 2024/02/17 16:57:34 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*ptr;

	ptr = (char *)s;
	while (*s)
	{
		if (*s == (char)c)
			return (ptr);
		s++;
		ptr++;
	}
	if ((char)c == '\0')
		return (ptr);
	ptr = NULL;
	return (ptr);
}
