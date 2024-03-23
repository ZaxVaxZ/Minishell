/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:23:45 by marvin            #+#    #+#             */
/*   Updated: 2024/03/23 05:23:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"

/* -----------------------
 * Functions in the file:
 *   count_words()
 *   free_and_return()
 *   is_separator()
 * -----------------------*/

/// @brief Count all parameters that are part of one command
/// @param q The parser queue
/// @return Returns their count
int	count_words(t_queue *q)
{
	int		count;
	t_queue	*prev;

	count = 0;
	prev = NULL;
	while (q && q->type != Op_logic && q->type != Semicolon
		&& q->type != Op_pipe)
	{
		if (q->type == Word && (!prev
				|| (prev->type != Op_redir && prev->type != Assign)))
			count++;
		prev = q;
		q = q->next;
	}
	return (count);
}

t_bool	free_and_return(t_queue **q, t_env **env, t_cmd **cmds, t_cmd *cmd)
{
	free_queue(q);
	free_cmd(cmds);
	free_env(env);
	free_cmd_node(cmd);
	return (False);
}

t_bool	is_separator(t_queue *node)
{
	return (node->type == Op_pipe || node->type == Op_logic
		|| node->type == Semicolon
		|| node->type == Bracket_open || node->type == Bracket_closed);
}
