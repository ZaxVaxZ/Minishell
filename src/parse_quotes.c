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

	if (!q || **s != DQ)
		return (True);
	if (*(*s + 1) == DQ)
		*s += 2;
	if (*(*s + 1) == DQ)
		return (True);
	if (!parse_op(q, s, DQ, 1))
		return (False);
	queue_end(*q)->type = Dq_open;
	wlen = 0;
	while ((*s)[wlen] && (*s)[wlen] != DQ)// parse every letter before the second double quote
	{
		if ((*s)[wlen] == DS)	// if the current character is dollar sign, parse it as a variable
		{
			if (!parse_op(q, s, DS, 1))	// parse the dollar sign operator; this will also skip the dollar sign
				return (False);
			if (!parse_word(q, s, 1))	// parse the word which is a potential variable name
				return (False);
			wlen = 0;
		}
		wlen++;
	}
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	s += wlen;
	return True;
	//if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
	//	return (False);
	//queue_end(*q)->type = Word;
	//*s += wlen;
	//if (!parse_op(q, s, SQ, 1))
	//	return (False);
	//queue_end(*q)->type = Sq_closed;
	//*s += 1;
	//return (True);
}