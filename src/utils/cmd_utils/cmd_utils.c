/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:23:45 by marvin            #+#    #+#             */
/*   Updated: 2024/07/23 14:57:15 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"
#include "ft_printf.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   count_words()
 *   free_and_return()
 *   is_separator()
 *   prep_cmd()
 * -----------------------*/

/// @brief Count all parameters that are part of one command
/// @param q The parser queue
/// @return Returns their count
static int	count_words(t_queue *q)
{
	int		count;

	count = 0;
	while (q && !is_separator(q))
	{
		if (q->type == Word)
			count++;
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

t_bool	prep_cmd(t_queue *q, t_cmd **node)
{
	char	**params;
	int		in;
	int		out;

	params = malloc(sizeof(char *) * (count_words(q) + 1));
	if (!params)
		return (False);
	*node = new_cmd_node(params, &q);
	in = 0;
	out = 0;
	if (!*node)
	{
		free(params);
		return (False);
	}
	params[0] = NULL;
	if (!count_redirs(q, &in, &out))
		return (True);
	if (in_out_mallocs(q, node, in, out) == False)
		return (False);
	return (True);
}

t_bool	is_separator(t_queue *node)
{
	return (node->type == Op_pipe || node->type == Op_logic
		|| node->type == Semicolon
		|| node->type == Bracket_open || node->type == Bracket_closed);
}
