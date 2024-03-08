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

void	delete_next(t_queue **q)
{
	t_queue	*tmp;

	if (!(*q)->next)
		return ;
	tmp = (*q)->next;
	(*q)->next = tmp->next;
	free(tmp->s);
	free(tmp);
}

void	check_illegal(t_queue *q)
{
	t_queue	*tmp;

	tmp = q;
	while (q)
	{
		if (q->type == Illegal)
		{
			write(2, "syntax error near unexpected token `", 36);
			write(2, q->s, ft_strlen(q->s));
			write(2, "'\n", 2);
			free_queue(&tmp);
			exit (2);
		}
		q = q->next;
	}
}

void	check_opens(t_queue **q, t_bool open_par, t_bool open_sq, t_bool open_dq)
{
	if (!open_par)
	{
		write(2, "syntax error due to missing token `)`\n", 38);
		free_queue(q);
		exit (2);
	}
	if (!open_sq)
	{
		write(2, "syntax error due to missing token `'`\n", 38);
		free_queue(q);
		exit (2);
	}
	if (!open_dq)
	{
		write(2, "syntax error due to missing token `\"`\n", 38);
		free_queue(q);
		exit (2);
	}
}

void	parse_clean_up(t_queue **q)
{
	t_bool	open_par;
	t_bool	open_sq;
	t_bool	open_dq;

	check_illegal(*q);
	while (*q)
	{
		if (open_sq && (*q)->next->type == Sq_open)
			delete_next(&q);
		if (open_dq && (*q)->next->type == Dq_open)
			delete_next(&q);
		open_sq = ((open_sq && (*q)->type != Sq_closed)
			|| ((*q)->type == Sq_open));
		open_dq = ((open_dq && (*q)->type != Dq_closed)
			|| ((*q)->type == Dq_open));
		open_par = ((open_par && (*q)->type != Bracket_closed)
			|| ((*q)->type == Bracket_open));
	}
	check_opens(q, open_par, open_sq, open_dq);
}