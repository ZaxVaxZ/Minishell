/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:21:55 by ehammoud          #+#    #+#             */
/*   Updated: 2024/03/02 20:12:53 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/* -----------------------
 * Functions in the file:
 *   parser()
 *   parse_command()
 *   parse_control()
 *   parse_assigns()
 *   parse_word()
 * -----------------------*/

/// @brief Parse variable assigns until a non-assign
/// @param q The currently built parse queue
/// @param s What's left unparsed of the string
/// @return Returns False if a malloc, True otherwise
static t_bool	parse_assigns(t_queue **q, char **s)
{
	char	*prev_s;

	if (!s || !*s || !**s || !q)
		return (True);
	if (*q && queue_end(*q)->type != Assign && !is_control_op(queue_end(*q)->s))
		return (True);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!grab_assign(q, s))
			return (False);
	}
	return (True);
}

/// @brief Parse non-meta characters up to a space or tab
/// @param q The currently built parse queue
/// @param s What's left unparsed of the string
/// @param var_name If True, only parse chars legal in var names
/// @return Returns False if a malloc, True otherwise
static t_bool	parse_word(t_queue **q, char **s, t_bool var_name)
{
	int		wlen;
	t_bool	valid_name;

	wlen = find_word_end(s, &valid_name, var_name);
	if ((*s)[wlen] == DS && ((*s)[wlen + 1] == SPACE || (*s)[wlen + 1] == TAB))
		wlen++;
	if (!wlen && var_name)
		wlen = 1;
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
	if (wlen && valid_name)
		queue_end(*q)->type = Name;
	else if (wlen)
		queue_end(*q)->type = Word;
	*s += wlen;
	return (True);
}

/// @brief Parse a full command (Assigns, IO Redirect, Command & params)
/// @param q The currently built parse queue
/// @param s What's left unparsed of the string
/// @return Returns False if a malloc, True otherwise
static t_bool	parse_command(t_queue **q, char **s)
{
	char	*prev_s;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	while (**s == SPACE || **s == TAB)
		(*s)++;
	if (!parse_assigns(q, s))
		return (False);
	prev_s = NULL;
	while (prev_s != *s)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!parse_op(q, s, INF, 2))
			return (False);
		if (!parse_op(q, s, OUF, 2))
			return (False);
		if (!parse_op(q, s, DS, 1))
			return (False);
		if (!parse_word(q, s, False))
			return (False);
	}
	return (True);
}

/// @brief Parse a single control operator if there is one
/// @param q The currently built parse queue
/// @param s What's left unparsed of the string
/// @return Returns False if a malloc, True otherwise
static t_bool	parse_control(t_queue **q, char **s)
{
	char	*prev_s;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	prev_s = NULL;
	while (prev_s != *s && **s != NL)
	{
		while (**s == SPACE || **s == TAB)
			(*s)++;
		prev_s = *s;
		if (!parse_op(q, s, LP, 1))
			return (False);
		if (!parse_op(q, s, AND, 2))
			return (False);
		if (!parse_op(q, s, PIPE, 2))
			return (False);
		if (!parse_op(q, s, RP, 1))
			return (False);
		if (!parse_op(q, s, SC, 1))
			return (False);
	}
	return (True);
}

/// @brief Parse a full line string into a queue
/// @param s Full line of input
/// @return Queue of parsed and labeled tokens
t_queue	*parse(char *s)
{
	char	*prev_s;
	t_queue	*q;

	if (!s)
		return (NULL);
	q = NULL;
	prev_s = NULL;
	while (prev_s != s)
	{
		prev_s = s;
		if (!parse_single_quote(&q, &s))
			return (NULL);
		if (!parse_double_quote(&q, &s))
			return (NULL);
		if (!parse_command(&q, &s))
			return (NULL);
		if (!parse_single_quote(&q, &s))
			return (NULL);
		if (!parse_double_quote(&q, &s))
			return (NULL);
		if (!parse_control(&q, &s))
			return (NULL);
	}
	return (q);
}