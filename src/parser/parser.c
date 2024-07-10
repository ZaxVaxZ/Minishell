/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 13:21:55 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/10 18:43:09 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   parse_op()
 *   parse_assign()
 *   parse_command()
 *   parse_control()
 *   parse()
 * -----------------------*/

/// @brief Check for and parse an operator at the start of the string
/// @param q The parse queue
/// @param s The unparsed string
/// @param op The operator to parse
/// @param max_occurs The max number of continuous occurances parsed
/// @return False if any malloc fails, True otherwise
t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs)
{
	int		occurs;
	char	*sub;

	if (q && *q && queue_end(*q)->type == Illegal)
		return (True);
	occurs = op_occur(op, *s);
	if (!occurs && op == SPACE)
		occurs = op_occur(TAB, *s);
	if (occurs == 1 && op == AND)
		return (True);
	if (occurs > max_occurs)
		occurs = max_occurs;
	sub = ft_substr(*s, 0, occurs);
	if (!sub)
		return (False);
	if (!add_str_to_queue(q, sub))
		return (False);
	*s += occurs;
	if (op == DS && occurs > 0 && (is_valid_var_char(**s) || **s == '?'))
		parse_word(q, s, True);
	else if (op == DS && occurs > 0)
		queue_end(*q)->type = Word;
	return (True);
}

/// @brief Parse a variable assign
/// @param q The currently built parse queue
/// @param s What's left unparsed of the string
/// @return Returns False if a malloc, True otherwise
static t_bool	parse_assign(t_queue **q, char **s)
{
	int		wlen;
	t_queue	*tmp;
	t_bool	assign_b4;

	tmp = *q;
	assign_b4 = (!tmp || tmp->type == Assign);
	while (tmp)
	{
		if (tmp->type == Assign || is_meta_char(tmp->s, True))
			assign_b4 = True;
		if (tmp->type == Whitespace && tmp->next && tmp->next->type != Assign)
			assign_b4 = False;
		tmp = tmp->next;
	}
	if (!s || !*s || !**s || found_in(**s, DIGIT) || !assign_b4)
		return (True);
	wlen = 0;
	while (is_valid_var_char((*s)[wlen]))
		wlen++;
	if ((*s)[wlen++] != EQUAL)
		return (True);
	if (!add_str_to_queue(q, ft_substr(*s, 0, wlen)))
		return (False);
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
	if (!parse_assign(q, s))
		return (False);
	prev_s = NULL;
	while (prev_s != *s)
	{
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
		if (q && queue_end(q)->type != Whitespace)
			parse_op(&q, &s, SPACE, 1);
		while (*s == SPACE || *s == TAB)
			s++;
		if (!is_legal_queue_end(q, s))
			return (q);
		prev_s = s;
		if (!parse_single_quote(&q, &s) || !parse_double_quote(&q, &s))
			return (NULL);
		if (!parse_command(&q, &s) || !parse_control(&q, &s))
			return (NULL);
	}
	return (q);
}
