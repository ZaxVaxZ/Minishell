/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:35 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/01 19:04:06 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	op_occur(char c, char *s)
{
	int	cc;

	cc = 0;
	if (!s || !*s)
		return (-1);
	while (*s == c)
	{
		s++;
		cc++;
		if (cc > 2)
			return (-1);
	}
	return (cc);
}

// allow_under_score might be unnecessary? as the parsing is indiscriminate
t_bool	is_literal(char c, t_bool allow_under_score)
{
	int	i;

	if (allow_under_score && c == UNDERSCORE)
		return (True);
	i = 0;
	while (i < 26)
	{
		if (c == UPPERCASE[i] || c == LOWERCASE[i])
			return (True);
		i++;
	}
	return (False);
}

char	*grab_word(char **s, t_bool start_under_score)
{
	int		i;
	int		wlen;
	char	*word;

	if (!s || !*s || !**s)
		return (NULL);
	wlen = 0;
	while (**s && is_literal(**s, start_under_score || (wlen > 0)))
		wlen++;
	word = malloc(sizeof(char) * (wlen + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < wlen)
	{
		word[i] = *s[i];
		i++;
	}
	word[i] = '\0';
	*s += wlen;
	return (word);
}
