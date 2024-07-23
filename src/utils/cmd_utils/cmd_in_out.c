/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_in_out.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 14:50:31 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/23 14:56:41 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"

t_bool	in_out_mallocs(t_queue *q, t_cmd **node, int in, int out)
{
	if (in)
	{
		(*node)->in_flags = malloc(sizeof(int) * in);
		if (!(*node)->in_flags)
			return (free_cmd_node(*node));
		(*node)->infiles = malloc(sizeof(char *) * (in + 1));
		if (!(*node)->infiles)
			return (free_cmd_node(*node));
	}
	if (out)
	{
		(*node)->out_flags = malloc(sizeof(int) * out);
		if (!(*node)->out_flags)
			return (free_cmd_node(*node));
		(*node)->outfiles = malloc(sizeof(char *) * (out + 1));
		if (!(*node)->outfiles)
			return (free_cmd_node(*node));
	}
	return (True);
}

/// @brief counts the infile and outfile redirs and sets their counters accordingly
/// @param q the queue to traverse
/// @param in the pointer to the in count
/// @param out the pointer to the out count
/// @return returns 0 if no infile or outfile; 1 for infile; 2 for out; 3 for both
int	count_redirs(t_queue *q, int *in, int *out)
{
	int	has_inf;
	int	has_outf;

	has_inf = 0;
	has_outf = 0;
	while (q && !is_separator(q))
	{
		if (q->type == Op_redir && q->s[0] == '>' && q->next)
		{
			(*out)++;
			has_outf = 2;
		}
		else if (q->type == Op_redir && q->s[0] == '<' && q->next)
		{
			(*in)++;
			has_inf = 1;
		}
		q = q->next;
	}
	return (has_inf + has_outf);
}
