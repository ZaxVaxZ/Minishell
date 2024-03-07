/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:12:10 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/05 13:20:13 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_bool	parse_single_quote(t_queue **q, char **s)
{
	int		wlen;

	if (!q || !s || !*s || **s != SQ)
		return (True);
	if (*(*s + 1) == SQ)
		*s += 2;
	if (*(*s + 1) == SQ)
		return (True);
	if (!parse_op(q, s, SQ, 1))
		return (False);
	queue_end(*q)->type = Sq_open;
	wlen = 0;
	while ((*s)[wlen] && (*s)[wlen] != SQ)
		wlen++;
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	queue_end(*q)->type = Word;
	*s += wlen;
	if (!parse_op(q, s, SQ, 1))
		return (False);
	queue_end(*q)->type = Sq_closed;
	*s += 1;
	return (True);
}

t_bool	parse_inside_dq(t_queue **q, char **s)
{
	int	wlen;

	wlen = 0;
	while ((*s)[wlen] && (*s)[wlen] != DQ)
	{
		if ((*s)[wlen++] == DS)
		{
			if (!add_str_to_queue(q, ft_substr(*s, 0, wlen - 1)))
				return (False);
			*s += wlen - 1;
			if (!parse_op(q, s, DS, 1))
				return (False);
			if (!parse_word(q, s, True))
				return (False);
			wlen = 0;
		}
	}
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	*s += wlen;
	return (True);
}

t_bool	parse_double_quote(t_queue **q, char **s)
{
	if (!q || !s || !*s || **s != DQ)
		return (True);
	if (*(*s + 1) == DQ)
		*s += 2;
	if (*(*s + 1) == DQ)
		return (True);
	if (!parse_op(q, s, DQ, 1))
		return (False);
	queue_end(*q)->type = Dq_open;
	if (!parse_inside_dq(q, s))
		return (False);
	if (!parse_op(q, s, DQ, 1))
		return (False);
	queue_end(*q)->type = Dq_closed;
	return (True);
}
