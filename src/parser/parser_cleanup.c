/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 04:46:55 by marvin            #+#    #+#             */
/*   Updated: 2024/03/08 04:46:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static void	delete_next(t_queue **q)
{
	t_queue	*tmp;

	if (!(*q)->next)
		return ;
	tmp = (*q)->next;
	(*q)->next = tmp->next;
	free(tmp->s);
	free(tmp);
}

static t_bool	join_quote(t_queue *q, t_token type)
{
	char	*addition;

	if (!q || !q->next)
		return (True);
	q->type = Word;
	free(q->s);
	q->s = ft_strdup("");
	if (!q->s)
		return (False);
	while (q->next && ((type == Sq_open && q->next->type != Sq_closed)
			|| (type == Dq_open && q->next->type != Dq_closed)))
	{
		addition = q->next->s;
		if (q->next->type == Variable)
			delete_next(&q);
		if (q->next->type == Name)
			addition = getenv(q->next->s);
		q->s = ft_strjoin(q->s, addition);
		if (!q->s)
			return (False);
		delete_next(&q);
	}
	delete_next(&q);
	return (True);
}

static t_bool	join_words(t_queue **h)
{
	t_queue	*q;

	q = *h;
	while (q)
	{
		if ((q->type == Sq_open || q->type == Dq_open)
			&& !join_quote(q, q->type))
			return (free_queue(h));
		q = q->next;
	}
	q = *h;
	while (q)
	{
		while (q->type == Word && q->next && q->next->type == Word)
		{
			q->s = ft_strjoin(q->s, q->next->s);
			if (!q->s)
				return (free_queue(h));
			delete_next(&q);
		}
		q = q->next;
	}
	return (True);
}

static t_bool	unpack(t_queue **h, int *open)
{
	t_queue	*q;

	if (open[0])
		return (syntax_error(h, "'", True));
	if (open[1])
		return (syntax_error(h, "\"", True));
	if (open[2])
		return (syntax_error(h, ")", True));
	q = *h;
	while (q)
	{
		if (q->type == Variable)
		{
			q->type = Word;
			free(q->s);
			q->s = ft_strdup("");
			if (getenv(q->next->s))
				q->s = ft_strdup(getenv(q->next->s));
			if (!q->s)
				return (free_queue(h));
			delete_next(&q);
		}
		q = q->next;
	}
	return (True);
}

int	parse_clean_up(t_queue **h)
{
	int		open[3];
	t_queue	*q;

	if (!h || !*h)
		return (-1);
	open[0] = 0;
	open[1] = 0;
	open[2] = 0;
	q = *h;
	if (queue_end(q)->type == Illegal)
		return (syntax_error(h, queue_end(q)->s, False));
	while (q)
	{
		open[0] += (q->type == Sq_open);
		open[0] -= (q->type == Sq_closed);
		open[1] += (q->type == Dq_open);
		open[1] -= (q->type == Dq_closed);
		open[2] += (q->type == Bracket_open);
		open[2] -= (q->type == Bracket_closed);
		q = q->next;
	}
	if (!unpack(h, open) || !join_words(h))
		return (-1);
	return (0);
}
