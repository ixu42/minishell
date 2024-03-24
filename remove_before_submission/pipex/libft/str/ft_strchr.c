/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 18:30:29 by ixu               #+#    #+#             */
/*   Updated: 2023/11/07 11:59:55 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

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
