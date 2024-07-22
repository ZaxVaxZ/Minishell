/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_extras.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 17:43:35 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/22 14:34:40 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   parse_inside_dq()
 *   parse_double_quote()
 *   parse_inside_sq()
 *   parse_single_quote()
 *   parse_word()
 * -----------------------*/

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
		if ((*s)[wlen] == DS && is_valid_var_char((*s)[wlen + 1]))
		{
			if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
				return (False);
			*s += wlen;
			if (!parse_op(q, s, DS, 1))
				return (False);
			wlen = 0;
		}
		else
			wlen++;
	}
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	if (wlen)
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
	if (!q || !s || !*s || **s != DQ || (*q && queue_end(*q)->type == Illegal))
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
	if (ft_strncmp(queue_end(*q)->s, "\"", -1))
		return (True);
	queue_end(*q)->type = Dq_closed;
	if (!parse_op(q, s, SPACE, 1))
		return (False);
	return (True);
}

/// @brief Helper function to parse string sequence inside of single quotes 
/// @param q The parse queue
/// @param s The unparsed string
/// @return False if any malloc fails, True otherwise.
t_bool	parse_inside_sq(t_queue **q, char **s)
{
	int	wlen;

	wlen = 0;
	while ((*s)[wlen] && (*s)[wlen] != SQ)
		wlen++;
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	queue_end(*q)->type = Word;
	*s += wlen;
	return (True);
}

/// @brief Parse single quotes and the string sequence inside
/// @param q The parse queue
/// @param s The unparsed string
/// @return False if any malloc fails, True otherwise.
t_bool	parse_single_quote(t_queue **q, char **s)
{
	if (!q || !s || !*s || **s != SQ || (*q && queue_end(*q)->type == Illegal))
		return (True);
	if (*(*s + 1) == SQ)
	{
		*s += 2;
		return (True);
	}
	if (!parse_op(q, s, SQ, 1))
		return (False);
	queue_end(*q)->type = Sq_open;
	if (!parse_inside_sq(q, s))
		return (False);
	if (!parse_op(q, s, SQ, 1))
		return (False);
	if (ft_strncmp(queue_end(*q)->s, "\'", -1))
		return (True);
	queue_end(*q)->type = Sq_closed;
	if (!parse_op(q, s, SPACE, 1))
		return (False);
	return (True);
}

/// @brief Parse non-meta characters up to a space or tab
/// @param q The currently built parse queue
/// @param s What's left unparsed of the string
/// @param var_name If True, only parse chars legal in var names
/// @return Returns False if a malloc, True otherwise
t_bool	parse_word(t_queue **q, char **s, t_bool var_name)
{
	int		wlen;
	t_bool	valid_name;

	wlen = 0;
	valid_name = !found_in((*s)[0], DIGIT);
	while (is_allowed_in_word(*s + wlen, valid_name, var_name))
		valid_name = is_valid_var_char((*s)[++wlen]);
	while ((*s)[wlen] == DS && !var_name && ((*s)[wlen + 1] == SPACE
			|| (*s)[wlen + 1] == TAB || is_meta_char(*s + wlen + 1, False)))
		wlen++;
	if (found_in((*s)[wlen], DIGIT))
		wlen++;
	if (!wlen && var_name && (*s)[wlen] == '?')
		wlen++;
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	if (var_name && wlen)
		queue_end(*q)->type = Name;
	 else if (wlen && queue_end(*q)->type != Illegal)
	 	queue_end(*q)->type = Word;
	*s += wlen;
	return (True);
}
