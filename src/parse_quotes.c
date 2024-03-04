/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 13:12:10 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/04 13:53:43 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_bool	parse_single_quote(t_queue **q, char **s)
{
	int		wlen;

	if (!q || **s != SQ)
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

t_bool	parse_double_quote(t_queue **q, char **s)
{
	int		wlen;

	if (!q || **s != SQ)
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