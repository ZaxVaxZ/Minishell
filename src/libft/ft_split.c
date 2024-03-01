/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:36:54 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 15:35:50 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_sep(char c, char *sep)
{
	while (*sep)
	{
		if (c == *sep)
			return (1);
		sep++;
	}
	return (0);
}

static int	word_count(char *s, char *sep)
{
	size_t	i;
	size_t	c;
	size_t	n;

	if (!s)
		return (0);
	n = 0;
	i = 0;
	c = 0;
	while (s[i] != '\0')
	{
		if (is_sep(s[i], sep) && c > 0)
		{
			n++;
			c = 0;
		}
		else if (!is_sep(s[i], sep))
			c++;
		i++;
	}
	if (c > 0)
		n++;
	return (n);
}

static char	**ft_freeup(char **arr)
{
	size_t	i;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return (NULL);
}

static char	**populate(char **ret, char *str, char *sep)
{
	size_t	i;
	size_t	j;
	size_t	wc;

	if (!ret || !str)
		return (NULL);
	wc = 0;
	i = 0;
	while (str[i] != '\0')
	{
		j = 0;
		while (str[i + j] && !is_sep(str[i + j], sep))
			j++;
		if (j > 0)
		{
			ret[wc++] = ft_substr(str, i, j);
			if (ret[wc - 1] == NULL)
				return (ft_freeup(ret));
		}
		i += j + (j == 0);
	}
	ret[wc] = NULL;
	return (ret);
}

char	**ft_split(char *s, char *sep)
{
	char	**ret;

	if (!s)
		return (NULL);
	ret = malloc((word_count(s, sep) + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	ret = populate(ret, s, sep);
	return (ret);
}
