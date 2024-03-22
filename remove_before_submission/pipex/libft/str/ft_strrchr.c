/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 19:08:12 by ixu               #+#    #+#             */
/*   Updated: 2023/11/06 15:44:15 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

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
