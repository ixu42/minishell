/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heapsort_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 11:06:25 by apimikov          #+#    #+#             */
/*   Updated: 2024/03/24 11:06:28 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// i is current root node, and n is total number of el in arr
void	heapify(char **arr, int n, int i)
{
	int		largest;
	int		left;
	int		right;
	char	*temp;

	largest = i;
	left = 2 * i + 1;
	right = 2 * i + 2;
	if (left < n && ft_strcmp(arr[left], arr[largest]) > 0)
		largest = left;
	if (right < n && ft_strcmp(arr[right], arr[largest]) > 0)
		largest = right;
	if (largest != i)
	{
		temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;
		heapify(arr, n, largest);
	}
}

void	heapsort_str(char **arr, int n)
{
	int		i;
	char	*temp;

	i = n / 2 - 1;
	while (i >= 0)
		heapify(arr, n, i--);
	i = n - 1;
	while (i > 0)
	{
		temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;
		heapify(arr, i--, 0);
	}
}
