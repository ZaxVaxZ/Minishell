/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:35 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:38:24 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   grab_assign()
 *   parse_inside_dq()
 *   parse_double_quote()
 *   parse_single_quote()
 *   parse_op()
 * -----------------------*/

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

/// @brief Helper function to parse string sequence inside of double quotes 
/// @param q The parse queue
/// @param s The unparsed string
/// @return False if any malloc fails, True otherwise.
static t_bool	parse_inside_dq(t_queue **q, char **s)
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
	queue_end(*q)->type = Word;
	*s += wlen;
	return (True);
}

/// @brief Parse double quotes and the string sequence inside
/// @param q The parse queue
/// @param s The unparsed string
/// @return False if any malloc fails, True otherwise.
t_bool	parse_double_quote(t_queue **q, char **s)
{
	if (!q || !s || !*s || **s != DQ)
		return (True);
	if (*(*s + 1) == DQ)
	{
		*s += 2;
		return (True);
	}
	if (!parse_op(q, s, DQ, 1))
		return (False);
	queue_end(*q)->type = Dq_open;
	if (!parse_inside_dq(q, s))
		return (False);
	if (!parse_op(q, s, DQ, 1))
		return (False);
	queue_end(*q)->type = Dq_closed;
	if (!parse_op(q, s, SPACE, 1))
		return (False);
	return (True);
}

/// @brief Parse single quotes and the string sequence inside
/// @param q The parse queue
/// @param s The unparsed string
/// @return False if any malloc fails, True otherwise.
t_bool	parse_single_quote(t_queue **q, char **s)
{
	int		wlen;

	if (!q || !s || !*s || **s != SQ)
		return (True);
	if (*(*s + 1) == SQ)
	{
		*s += 2;
		return (True);
	}
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
	if (!parse_op(q, s, SPACE, 1))
		return (False);
	return (True);
}

/// @brief Check for and parse an operator at the start of the string
/// @param q The parse queue
/// @param s The unparsed string
/// @param op The operator to parse
/// @param max_occurs The max number of continuous occurances parsed
/// @return False if any malloc fails, True otherwise
t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs)
{
	int	occurs;

	occurs = op_occur(op, *s);
	if (!occurs && op == SPACE)
		occurs = op_occur(TAB, *s);
	if (occurs == 1 && op == AND)
		return (True);
	if (occurs > max_occurs)
		occurs = max_occurs;
	if (!add_str_to_queue(q, ft_substr(*s, 0, occurs)))
		return (False);
	*s += occurs;
	if (op == DS && occurs > 0 && (**s == SPACE || **s == TAB))
		queue_end(*q)->type = Word;
	else if (op == DS && occurs > 0 && !is_control_op(*s))
		parse_word(q, s, True);
	return (True);
}
