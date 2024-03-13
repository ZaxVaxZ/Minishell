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

void	syntax_error(t_queue **q, char *token, t_bool missing)
{
	if (missing)
		write(2, "syntax error due to missing token `", 35);
	else
		write(2, "syntax error near unexpected token `", 36);
	write(2, token, ft_strlen(token));
	write(2, "`\n", 2);
	free_queue(q);
	exit (2);
}

void	parse_clean_up(t_queue **q)
{
	t_bool	open[3];

	if (queue_end(*q)->type == Illegal)
		syntax_error(q, queue_end(*q)->s, False);
	while (*q)
	{
		if (open[0] && (*q)->next->type == Sq_open)
			delete_next(&q);
		if (open[1] && (*q)->next->type == Dq_open)
			delete_next(&q);
		open[0] = ((open[0] && (*q)->type != Sq_closed)
			|| ((*q)->type == Sq_open));
		open[1] = ((open[1] && (*q)->type != Dq_closed)
			|| ((*q)->type == Dq_open));
		open[2] = ((open[2] && (*q)->type != Bracket_closed)
			|| ((*q)->type == Bracket_open));
	}
}