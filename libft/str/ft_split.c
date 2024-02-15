/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 00:58:57 by ixu               #+#    #+#             */
/*   Updated: 2024/01/07 00:58:57 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static char	**split_empty_str(void)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * 2);
	if (arr != NULL)
	{
		arr[0] = (char *)malloc(sizeof(char));
		if (arr[0] == NULL)
			return (NULL);
		arr[0][0] = '\0';
		arr[1] = NULL;
	}
	return (arr);
}

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s == c)
		s++;
	while (*s)
	{
		if (*s == c)
		{
			count++;
			s++;
			while (*s == c)
				s++;
		}
		else
			s++;
	}
	s--;
	if (*s == c)
		return (count);
	else
		return (count + 1);
}

static void	free_arr(char **arr, int k)
{
	int	i;

	i = 0;
	while (i <= k)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	arr = NULL;
}

static char	**fill_arr(char const **ptr_to_s, char c, char **arr, int *ptr_to_k)
{
	int	len;
	int	i;

	len = 0;
	while ((*ptr_to_s)[len] != c && (*ptr_to_s)[len])
		len++;
	arr[*ptr_to_k] = (char *)malloc(sizeof(char) * (len + 1));
	if (arr[*ptr_to_k] == NULL)
	{
		free_arr(arr, *ptr_to_k);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		arr[*ptr_to_k][i] = (*ptr_to_s)[i];
		i++;
	}
	arr[*ptr_to_k][i] = '\0';
	while (*(*ptr_to_s) != c && *(*ptr_to_s))
		(*ptr_to_s)++;
	while (*(*ptr_to_s) == c && *(*ptr_to_s))
		(*ptr_to_s)++;
	(*ptr_to_k)++;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		k;

	if (ft_strlen(s) == 0)
		return (split_empty_str());
	arr = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (arr == NULL)
		return (NULL);
	k = 0;
	while (*s == c)
		s++;
	while (*s != '\0')
	{
		arr = fill_arr(&s, c, arr, &k);
		if (arr == NULL)
			return (NULL);
	}
	arr[k] = NULL;
	return (arr);
}
