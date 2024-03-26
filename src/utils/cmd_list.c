/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:13:59 by pipolint          #+#    #+#             */
/*   Updated: 2024/03/26 14:20:57 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"

/* -----------------------
 * Functions in the file:
 *   new_cmd_node()
 *   add_cmd_node()
 *   cmd_size()
 *   free_cmd_node()
 *   free_cmd()
 * -----------------------*/

/// @brief Create a new cmdironment list node
/// @param key Variable name as the key
/// @param value The value stored for the variable
/// @return Returns the created node. NULL if a malloc fails
t_cmd	*new_cmd_node(char **params)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->depth = 0;
	node->status = 0;
	node->in_fd = -1;
	node->input = NULL;
	node->out_fd = -1;
	node->ovrw_cnt = 0;
	node->apnd_cnt = 0;
	node->params_cnt = 0;
	node->ovrw_outs = NULL;
	node->apnd_outs = NULL;
	node->params = params;
	node->heredoc = False;
	node->or_op = False;
	node->before = Illegal;
	node->after = Illegal;
	node->next = NULL;
	return (node);
}

/// @brief Adds a cmd node to a cmd list
/// @param cmd The cmd list
/// @param to_add The node to be added
void	add_cmd_node(t_cmd **cmd, t_cmd *to_add)
{
	t_cmd	*trav;

	if (!cmd || !to_add)
		return ;
	if (!*cmd)
	{
		*cmd = to_add;
		return ;
	}
	trav = (*cmd);
	while (trav->next)
		trav = trav->next;
	trav->next = to_add;
}

/// @brief Calculates and returns the size of the cmd list
/// @param cmd The cmd list
/// @return The number of nodes in the list. 0 If empty
int	cmd_size(t_cmd *cmd)
{
	int	size;

	size = 0;
	while (cmd)
	{
		size++;
		cmd = cmd->next;
	}
	return (size);
}

/// @brief Free a cmd node and its contents
/// @param node The cmd node
t_bool	free_cmd_node(t_cmd *node)
{
	int	i;

	i = 0;
	while (node && node->params && node->params[i])
		free(node->params[i++]);
	if (node && node->params)
		free(node->params);
	if (node && node->input)
		free(node->input);
	i = 0;
	while (node && node->ovrw_outs && node->ovrw_outs[i])
		free(node->ovrw_outs[i++]);
	if (node && node->ovrw_outs)
		free(node->ovrw_outs);
	i = 0;
	while (node && node->apnd_outs && node->apnd_outs[i])
		free(node->apnd_outs[i++]);
	if (node && node->apnd_outs)
		free(node->apnd_outs);
	if (node)
		free(node);
	return (False);
}

/// @brief Free each node of a cmd list, and their contents
/// @param cmd The cmd list pointer passed by address
/// @return Returns False to be used as a return value for caller function
t_bool	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	while (cmd && *cmd)
	{
		tmp = *cmd;
		*cmd = (*cmd)->next;
		free_cmd_node(tmp);
	}
	*cmd = NULL;
	return (False);
}
