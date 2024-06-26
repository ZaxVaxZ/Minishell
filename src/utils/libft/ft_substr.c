/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:10:59 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 20:56:11 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*news;
	size_t	i;
	size_t	j;
	size_t	n;

	if (!s)
		return (NULL);
	if (!len)
		return (ft_strdup(""));
	n = ft_strlen(s);
	if (start >= n || s[0] == '\0')
		return (ft_strdup(""));
	if (start + len > n)
		j = n - start;
	else
		j = len;
	news = malloc((j + 1) * sizeof(char));
	if (!news)
		return (NULL);
	j = 0;
	i = start;
	while (s[i] != '\0' && j < len)
		news[j++] = s[i++];
	news[j] = '\0';
	return (news);
}
