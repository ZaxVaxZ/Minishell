/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 05:23:45 by marvin            #+#    #+#             */
/*   Updated: 2024/04/01 16:48:03 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   count_words()
 *   free_and_return()
 *   is_separator()
 * -----------------------*/

/// @brief Count all parameters that are part of one command
/// @param q The parser queue
/// @return Returns their count
static int	count_words(t_queue *q)
{
	int		count;
	//t_queue	*prev;

	count = 0;
	//prev = NULL;
	while (q && q->type != Op_logic && q->type != Semicolon
		&& q->type != Op_pipe)
	{
		if (q->type == Word)
			count++;
		//prev = q;
		q = q->next;
	}
	return (count);
}

int	count_out_redirs(t_queue *q)
{
	int		redirs;

	redirs = 0;
	while (q && !is_separator(q))
	{
		//if (q->type == Op_redir && q->next && !(q->next->type == Name))
		if (q->type == Op_redir && q->next)
			redirs++;
		q = q->next;
	}
	return (redirs);
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

	params = malloc(sizeof(char *) * (count_words(q) + 1));
	if (!params)
		return (False);
	*node = new_cmd_node(params);
	if (!*node)
	{
		free(params);
		return (False);
	}
	else
		params[0] = NULL;
	if (!count_out_redirs(q))
		return (True);
	(*node)->outfiles = malloc(sizeof(char *) * (count_out_redirs(q) + 1));
	(*node)->out_flags = malloc(sizeof(int) * count_out_redirs(q));
	if (!(*node)->outfiles || !(*node)->out_flags)
		return (free_cmd_node(*node));
	(*node)->outfiles[0] = NULL;
	return (True);
}

t_bool	is_separator(t_queue *node)
{
	return (node->type == Op_pipe || node->type == Op_logic
		|| node->type == Semicolon
		|| node->type == Bracket_open || node->type == Bracket_closed);
}
