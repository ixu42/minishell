#include "../minishell.h"
	
int	strlist_len(t_strcmd *str)
{
	int i;

	i = 0;
	while (str !=NULL)
	{
		i += str->end - str->start;
		str = str->next;
	}
	return (i);
}

char *strlist_join(t_strcmd *str)
{
	char	*res;
	int		len;
	int		i;
	int		j;
	
	len = strlist_len(str);
	res = (char *)malloc(sizeof(*res) * (len + 1));
	if (!res)
		return (NULL);
	ft_memset(res, 0, sizeof(*res) * (len + 1));
	i = 0;
	while (str !=NULL && i < len)
	{
		j = 0;
		while (j < str->end - str->start && str->start[j] != '\0')
			res[i++] = str->start[j++];
		str = str->next;
	}
	return (res);
}
/*
void	replace_var_nodes(t_strcmd *str, char *data)
{
	while (str !=NULL)
	{
		
		str = str->next;
	}
}
*/
