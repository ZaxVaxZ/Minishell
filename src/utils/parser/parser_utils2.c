/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:44:27 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/26 13:52:25 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	check_parentheses(t_queue *q, t_queue **q_end, int *opened, int *word)
{
	while (q)
	{
		*opened += (q->type == Bracket_open);
		*opened -= (q->type == Bracket_closed);
		if (!is_meta_char(q->s, True) && q->type != Bracket_open)
		{
			if (q->type != Whitespace)
				(*word)++;
		}
		else if (is_meta_char(q->s, True))
			*word = 0;
		if (q->type != Whitespace)
			*q_end = q;
		q = q->next;
	}
}

int	dont_unpack(t_queue *q)
{
	char	*tmp;
	char	*dollar;

	if (q->type == Op_redir && !ft_strncmp("<<", q->s, -1))
	{
		if (q->next->type == Whitespace)
			delete_next(&q);
		if (q->next->type == Variable)
		{
			delete_next(&q);
			dollar = ft_strdup("$");
			if (!dollar)
				return (-1);
			tmp = ft_strjoin(dollar, q->next->s);
			if (!tmp)
				return (-1);
			free(q->next->s);
			q->next->s = tmp;
		}
		return (1);
	}
	return (0);
}

void	set_zero(void *arr, size_t size)
{
	unsigned char	*arr_p;
	size_t			n;

	arr_p = (unsigned char *)arr;
	n = -1;
	while (++n < size)
		arr_p[n] = 0;
}

void	set_non_whitespace(t_queue **q, t_queue *last, t_bool ill, char *str)
{
	t_queue	*tmp;

	tmp = new_node(str);
	if (!tmp)
		free_queue(q);
	else
	{
		tmp->type = token_type(tmp->s);
		if (ill || (last && last->type == Bracket_closed
				&& tmp->type == Word))
			tmp->type = Illegal;
		queue(q, tmp);
	}
	free(str);
}

int	free_queue_and_return(t_queue **q)
{
	free_queue(q);
	return (-1);
}
