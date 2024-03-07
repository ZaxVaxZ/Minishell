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

t_bool	add_str_to_queue(t_queue **q, char *str)
{
	t_bool	illegal;
	t_queue	*tmp;

	if (!q)
		return (False);
	if (!str)
		return (True);
	tmp = queue_end(*q);
	illegal = !is_legal(str, tmp);
	tmp = new_node(str);
	if (!tmp)
		free_queue(q);
	else
	{
		tmp->type = token_type(tmp->s);
		if (illegal)
			tmp->type = Illegal;
		queue(q, tmp);
	}
	free(str);
	if (!*q)
		return (False);
	return (True);
}

t_bool	parse_op(t_queue **q, char **s, char op, int max_occurs)
{
	int	occurs;

	occurs = op_occur(op, *s);
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

t_bool	parse_command(t_queue **q, char **s)
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

t_bool	parse_control(t_queue **q, char **s)
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
