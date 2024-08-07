/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 03:34:49 by marvin            #+#    #+#             */
/*   Updated: 2024/07/27 14:34:24 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"
#include "executor.h"
#include <stdio.h>

/* -----------------------
 * Functions in the file:
 *   before_cmd()
 *   after_cmd()
 *   queue_node_to_cmd()
 *   build_commands()
 * -----------------------*/

/// @brief Handles the open parenthesis and operators before a command
/// @param q The parser queue
/// @param tmp The cmd node being built
/// @param depth How deeply nested a command is in parenthesis
/// @return The new position in the queue after skipping to the command
static t_queue	*before_cmd(t_queue *q, t_cmd **cmds)
{
	t_cmd	*p;

	if (!q || !cmds)
		return (NULL);
	while (q && q->type == Bracket_open)
	{
		p = new_cmd_node(NULL, &q);
		if (!p)
			return (NULL);
		p->rep = LP;
		add_cmd_node(cmds, p);
		q = q->next;
	}
	return (q);
}

/// @brief Handles the closed parenthesis and operators after a command
/// @param q The parser queue
/// @param tmp The cmd node being built
/// @param depth How deeply nested a command is in parenthesis
/// @return The new position in the queue after skipping to the next command
static t_queue	*after_cmd(t_queue *q, t_cmd *tmp, t_cmd **cmds)
{
	t_cmd	*p;

	add_cmd_node(cmds, tmp);
	while (q && (q->type == Bracket_closed || q->type == Semicolon))
	{
		if (q->type == Bracket_closed)
		{
			p = new_cmd_node(NULL, &q);
			if (!p)
				return (NULL);
			p->rep = RP;
			add_cmd_node(cmds, p);
		}
		else
			tmp->after = SEMICOLON;
		q = q->next;
	}
	if (q && is_separator(q) && tmp->after == 0)
	{
		tmp->after = OR_OP * (!ft_strncmp(q->s, "||", -1));
		tmp->after += AND_OP * (!ft_strncmp(q->s, "&&", -1));
		tmp->after += PIPE_OP * (q->type == Op_pipe);
		q = q->next;
	}
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
		cmd->in_flags[cmd->infile_cnt] = ((*q)->s[1] == '<');
		if (cmd->in_flags[cmd->infile_cnt])
			cmd->heredoc = 1;
		cmd->infiles[cmd->infile_cnt] = ft_strdup((*q)->next->s);
		if (!cmd->infiles[cmd->infile_cnt++])
			return (False);
		cmd->infiles[cmd->infile_cnt] = NULL;
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
		if ((*q)->next && (*q)->next->type == Name)
		{
			if (!add_var(env, (*q)->s, (*q)->next->s))
				return (free_cmd_node(cmd));
		}
		else if (!add_var(env, (*q)->s, ""))
			return (free_cmd_node(cmd));
	}
	if ((*q) && (*q)->type == Word && (*q)->s[0] != '\0')
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
//t_bool	build_commands(t_queue **queue, t_cmd **cmds, t_env **env)
t_bool	build_commands(t_main *m)
{
	t_cmd	*tmp;
	t_queue	*q;
	int		tmp_before;

	tmp_before = NON;
	q = m->q;
	while (q)
	{
		q = before_cmd(q, &m->cmds);
		if (!prep_cmd(q, &tmp))
			return (free_and_return(&m->q, &m->env, &m->cmds, tmp));
		tmp->before = tmp_before;
		while (q && !is_separator(q))
		{
			if (!queue_node_to_cmd(&q, tmp, &m->env))
				return (free_and_return(&m->q, &m->env, &m->cmds, tmp));
			q = q->next;
		}
		q = after_cmd(q, tmp, &m->cmds);
		if (tmp->after)
			tmp_before = tmp->after;
	}
	return (True);
}
