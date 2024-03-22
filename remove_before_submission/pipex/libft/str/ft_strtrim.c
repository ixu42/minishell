/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 13:43:52 by ixu               #+#    #+#             */
/*   Updated: 2023/11/07 14:32:49 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	char_in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static int	chars_to_be_removed(char const *s1, char const *set)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s1[i] && char_in_set(s1[i], set))
	{
		count++;
		i++;
	}
	i = (int)ft_strlen(s1) - 1;
	while (count != (int)ft_strlen(s1) && char_in_set(s1[i], set))
	{
		count++;
		i--;
	}
	return (count);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*s1_cpy;
	int		s1_cpy_len;
	int		start;
	int		end;
	int		k;

	s1_cpy_len = (int)ft_strlen(s1) - chars_to_be_removed(s1, set) + 1;
	s1_cpy = (char *)malloc(sizeof(char) * s1_cpy_len);
	if (s1_cpy == NULL)
		return (NULL);
	end = (int)ft_strlen(s1) - 1;
	while (end >= 0 && char_in_set(s1[end], set))
		end--;
	start = 0;
	while (start <= end && char_in_set(s1[start], set))
		start++;
	k = 0;
	while (start <= end)
		s1_cpy[k++] = s1[start++];
	s1_cpy[k] = '\0';
	return (s1_cpy);
}
