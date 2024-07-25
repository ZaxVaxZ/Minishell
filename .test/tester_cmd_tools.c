/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 03:34:49 by marvin            #+#    #+#             */
/*   Updated: 2024/04/01 14:20:39 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"
#include <stdio.h>
#include "ft_printf.h"

/* -----------------------
 * Functions in the file:
 *   before_cmd()
 *   after_cmd()
 *   queue_node_to_cmd()
 *   build_commands()
 * -----------------------*/

 t_bool	printed_var_top = False;

/// @brief Handles the open parenthesis and operators before a command
/// @param q The parser queue
/// @param tmp The cmd node being built
/// @param depth How deeply nested a command is in parenthesis
/// @return The new position in the queue after skipping to the command
static t_queue	*before_cmd(t_queue *q, int *depth)
{
	if (!q)
		return (NULL);
	while (q && q->type == Bracket_open)
	{
		(*depth)++;
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
	while (q && q->type == Bracket_closed)
	{
		(*depth)--;
		q = q->next;
	}
	if (q && is_separator(q) && q->type != Bracket_open && tmp->after == Illegal)
	{
		tmp->after = q->type;
		if (q->type == Op_logic && !ft_strncmp(q->s, "||", -1))
			tmp->or_op = True;
	}
	if (q && is_separator(q) && q->type != Bracket_open)
		q = q->next;
	return (q);
}

/// @brief set the redirection parameters of the cmd node
/// @param q the queue node to go through
/// @param cmd the cmd node to modify
/// @return false if any malloc fails, true otherwise
static t_bool	queue_redir_to_cmd(t_queue **q, t_cmd *cmd)
{
	if ((*q) && (*q)->type == Op_redir && (*q)->next && (*q)->s[0] == '>')
	{
		cmd->out_flags[cmd->outfile_cnt] = ((*q)->s[1] == '>');
		cmd->outfiles[cmd->outfile_cnt] = ft_strdup((*q)->next->s);
		if (!cmd->outfiles[cmd->outfile_cnt++])
			return (False);
		cmd->outfiles[cmd->outfile_cnt] = NULL;
	}
	if ((*q) && (*q)->type == Op_redir && (*q)->next && (*q)->s[0] == '<')
	{
		cmd->heredoc = ((*q)->s[1] == '<');
		cmd->input = ft_strdup((*q)->next->s);
		if (!cmd->input)
			return (False);
	}
	if ((*q) && (*q)->type == Op_redir)
		(*q) = (*q)->next;
	return (True);
}

/// @brief Handle a parser node as either Assignment, Redirection or Parameter
/// @param q The parser node
/// @param cmd The cmd node for the command we are building
/// @param env The list of environment variables
/// @param i Index of the parameter in the parameter list of the cmd node
/// @return False if any malloc fails. True otherwise
static t_bool	queue_node_to_cmd(t_queue **q, t_cmd *cmd, t_env **env)
{
	if ((*q) && (*q)->type == Assign && (*q)->next)
	{
		if ((*q)->next->type == Name && !add_var(env, (*q)->s, (*q)->next->s))
			return (free_cmd_node(cmd));
		else if (!add_var(env, (*q)->s, ""))
			return (free_cmd_node(cmd));
		if (!printed_var_top)
		{
			ft_printf("\n|-|%-10.10s|%-10.10s|\n", " Var_Name", "  Value");
			printed_var_top = True;
		}
		ft_printf("|-|%-10.10s|%-10.10s|\n", (*q)->s, (*q)->next->s);
	}
	else if ((*q) && (*q)->type == Word)
	{
		cmd->params[cmd->params_cnt] = ft_strdup((*q)->s);
		if (!cmd->params[cmd->params_cnt++])
			return (free_cmd_node(cmd));
		cmd->params[cmd->params_cnt] = NULL;
	}
	if (!queue_redir_to_cmd(q, cmd))
		return (free_cmd_node(cmd));
	return (True);
}

/// @brief build the command node and add it to the cmd list
/// @param queue the queue to traverse through
/// @param cmds the cmd list
/// @param env the shell env
/// @return false if any malloc fails, true otherwise
t_bool	build_commands(t_queue **queue, t_cmd **cmds, t_env **env)
{
	int		depth;
	t_cmd	*tmp;
	t_queue	*q;

	depth = 0;
	q = *queue;
	while (q)
	{
		q = before_cmd(q, &depth);
		if (!prep_cmd(q, &tmp))
			return (free_and_return(queue, env, cmds, tmp));
		while (q && !is_separator(q))
		{
			if (!queue_node_to_cmd(&q, tmp, env))
				return (free_and_return(queue, env, cmds, tmp));
			q = q->next;
		}
		tmp->depth = depth;
		q = after_cmd(q, tmp, &depth);
		q = after_cmd(q, tmp, &depth);
		add_cmd_node(cmds, tmp);
	}
	return (True);
}
