/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 03:34:49 by marvin            #+#    #+#             */
/*   Updated: 2024/03/30 19:27:02 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   queue_node_to_cmd()
 *   before_cmd()
 *   after_cmd()
 *   build_commands()
 * -----------------------*/

/// @brief Handles the open parenthesis and operators before a command
/// @param q The parser queue
/// @param tmp The cmd node being built
/// @param depth How deeply nested a command is in parenthesis
/// @return The new position in the queue after skipping to the command
static t_queue	*before_cmd(t_queue *q, t_cmd *tmp, int *depth)
{
	if (!q || !tmp)
		return (NULL);
	if (is_separator(q) && q->type != Bracket_open)
	{
		tmp->before = q->type;
		if (q->type == Op_logic && !ft_strncmp(q->s, "||", -1))
			tmp->or_op = True;
		q = q->next;
	}
	while (q && q->type == Bracket_open)
	{
		*depth += (q->type == Bracket_open);
		q = q->next;
	}
	return (q);
}

/// @brief Handles the closed parenthesis and operators after a command
/// @param q The parser queue
/// @param tmp The cmd node being built
/// @param depth How deeply nested a command is in parenthesis
/// @return The new position in the queue after skipping to the next command
static t_queue	*after_cmd(t_queue *q, t_cmd *tmp, int *depth)
{
	tmp->depth = *depth;
	while (q && q->type == Bracket_closed)
	{
		*depth -= (q->type == Bracket_closed);
		q = q->next;
	}
	if (q && is_separator(q))
	{
		tmp->after = q->type;
		q = q->next;
	}
	return (q);
}

/// @brief Handle a parser node as either Assignment, Redirection or Parameter
/// @param q The parser node
/// @param cmd The cmd node for the command we are building
/// @param env The list of environment variables
/// @param i Index of the parameter in the parameter list of the cmd node
/// @return False if any malloc fails. True otherwise
static t_bool	queue_node_to_cmd(t_queue **q, t_cmd *cmd, t_env **env)
{
	if ((*q)->type == Assign && (*q)->next && (*q)->next->type == Word)
	{
		if (!add_var(env, (*q)->s, (*q)->next->s))
			return (free_cmd_node(cmd));
	}
	else if ((*q)->type == Op_redir && (*q)->s[0] == '<' && (*q)->next)
	{
		cmd->heredoc = ((*q)->s[1] == '<');
		cmd->input = ft_strdup((*q)->next->s);
		if (!cmd->input)
			return (free_cmd_node(cmd));
	}
	else if ((*q)->type == Op_redir && (*q)->next)
		cmd->outfiles[cmd->outfile_cnt] = ft_strdup((*q)->next->s);
	else if ((*q)->type == Word)
		cmd->params[cmd->params_cnt] = ft_strdup((*q)->s);
	if (((*q)->type == Op_redir && (!ft_strncmp((*q)->s, ">", -1) \
				|| !ft_strncmp((*q)->s, ">>", -1)) \
			&& !cmd->outfiles[cmd->outfile_cnt++]) \
		|| ((*q)->type == Word && !cmd->params[cmd->params_cnt++]))
		return (free_cmd_node(cmd));
	if ((*q)->type == Op_redir)
		(*q) = (*q)->next;
	return (True);
}

t_bool	build_commands(t_queue **queue, t_cmd **cmds, t_env **env)
{
	int		depth;
	t_cmd	*tmp;
	t_queue	*q;

	depth = 0;
	q = *queue;
	while (q)
	{
		if (!prep_cmd(q, &tmp))
			return (free_and_return(queue, env, cmds, tmp));
		q = before_cmd(q, tmp, &depth);
		while (q && !is_separator(q))
		{
			if (!queue_node_to_cmd(&q, tmp, env))
				return (free_and_return(queue, env, cmds, tmp));
			q = q->next;
		}
		tmp->params[tmp->params_cnt] = NULL;
		tmp->outfiles[tmp->outfile_cnt] = NULL;
		q = after_cmd(q, tmp, &depth);
		add_cmd_node(cmds, tmp);
	}
	return (True);
}
