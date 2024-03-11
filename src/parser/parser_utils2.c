/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 20:25:09 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/11 21:34:51 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/// @brief Helper function to parse a single assignment sequence
/// @param q The parse queue
/// @param s The unparsed string
/// @return False if any malloc fails, True otherwise.
t_bool	grab_assign(t_queue **q, char **s)
{
	int		wlen;
	t_bool	found_eq;

	if (!s || !*s || !**s || found_in(**s, DIGIT))
		return (True);
	found_eq = False;
	wlen = 0;
	while ((*s)[wlen] && (*s)[wlen] != SPACE && (*s)[wlen] != TAB
		&& !is_meta_char(*s + wlen))
	{
		if (!found_eq && !is_valid_var_char((*s)[wlen]))
			break ;
		wlen++;
		if (!found_eq && (*s)[wlen] == EQUAL)
			found_eq = True;
	}
	if (!found_eq)
		return (True);
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	*s += wlen;
	return (True);
}

t_bool	assign_before(t_queue *q)
{
	t_bool	ret;

	ret = True;
	while (q)
	{
		if (q->type == Assign || is_control_op(q->s))
			ret = True;
		if (q->type == Whitespace && q->next && q->next->type != Assign)
			ret = False;
		q = q->next;
	}
	return (ret);
}
