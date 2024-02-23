/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_charset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ixu <ixu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 00:44:41 by ixu               #+#    #+#             */
/*   Updated: 2024/01/07 00:44:41 by ixu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	is_sep(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		i++;
	}
	if (c == '\0')
		return (1);
	return (0);
}

static int	count_word(char *str, char *charset)
{
	int	word;
	int	i;

	word = 0;
	i = 0;
	while (str[i])
	{
		if (is_sep(str[i], charset) == 0
			&& is_sep(str[i + 1], charset) == 1)
			word++;
		i++;
	}
	return (word);
}

static char	*write_word(char *dest, char *str, char *charset)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (is_sep(str[i], charset) == 1)
			i++;
		while (is_sep(str[i], charset) == 0)
			dest[j++] = str[i++];
		break ;
	}
	dest[j] = '\0';
	return (dest);
}

static char	**write_arr(char **arr, char *str, char *charset)
{
	int	len;
	int	i;
	int	word;

	len = 0;
	i = 0;
	word = 0;
	while (str[i])
	{
		if (is_sep(str[i], charset) == 1)
			i++;
		else
		{
			while (is_sep(str[i + len], charset) == 0)
				len++;
			arr[word] = malloc(sizeof(char) * (len + 1));
			if (arr[word] == NULL)
				return (NULL);
			arr[word] = write_word(arr[word], str + i, charset);
			i += len;
			len = 0;
			word++;
		}
	}
	return (arr);
}

char	**ft_split_charset(char *str, char *charset)
{
	char	**arr;
	int		word;

	word = count_word(str, charset);
	arr = (char **)malloc((word + 1) * sizeof(char *));
	if (arr == NULL)
		return (NULL);
	arr = write_arr(arr, str, charset);
	arr[word] = NULL;
	return (arr);
}
