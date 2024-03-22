/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tmp_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 22:25:25 by marvin            #+#    #+#             */
/*   Updated: 2024/03/21 22:25:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tmp_utils.h"

char	*type_to_str(t_token type)
{
	if (type == Word)
		return ("Word");
	if (type == Name)
		return ("Name");
	if (type == Assign)
		return ("Assign");
	if (type == Op_pipe)
		return ("Op_pipe");
	if (type == Op_redir)
		return ("Op_redir");
	if (type == Op_logic)
		return ("Op_logic");
	if (type == Variable)
		return ("Variable");
	if (type == Semicolon)
		return ("Semicolon");
	if (type == Sq_open)
		return ("Sq_open");
	if (type == Sq_closed)
		return ("Sq_closed");
	if (type == Dq_open)
		return ("Dq_open");
	if (type == Dq_closed)
		return ("Dq_closed");
	if (type == Bracket_open)
		return ("Bracket_open");
	if (type == Bracket_closed)
		return ("Bracket_closed");
	if (type == Whitespace)
		return ("Whitespace");
	if (type == Illegal)
		return ("Illegal");
	return ("Unrecognized");
}

void	print_queue(t_queue *queue)
{
	if (!queue)
	{
		write(1, "The queue is empty!\n\n", 21);
		return ;
	}
	write(1, "|----------|----------|\n", 24);
	ft_printf("|%10.10s|%10.10s|", "  String", "   Type");
	write(1, "|----------|----------|\n", 24);
	while (queue)
	{
		ft_printf("|%10.10s|%10.10s|", queue->s, type_to_str(queue->type));
		queue = queue->next;
	}
	write(1, "|----------|----------|\n\n", 25);
}
