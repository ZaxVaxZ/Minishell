/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 15:13:59 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/15 18:34:35 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_list.h"

/* -----------------------
 * Functions in the file:
 *   new_cmd_node()
 *   add_cmd_node()
 *   delete_next_cmd()
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
	node->rep = '\0';
	node->status = 0;
	node->in_fd = -1;
	node->input = NULL;
	node->out_fd = -1;
	node->outfile_cnt = 0;
	node->out_flags = NULL;
	node->outfiles = NULL;
	node->params_cnt = 0;
	node->params = params;
	node->heredoc = False;
	node->or_op = False;
	node->before = 0;
	node->after = 0;
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

/// @brief Deletes the command node following the current one
/// @param cmd The current command node
void	delete_next_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;

	if (!cmd || !*cmd || !(*cmd)->next)
		return ;
	tmp = (*cmd)->next;
	(*cmd)->next = (*cmd)->next->next;
	free_cmd_node(tmp);
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
	while (node && node->outfiles && node->outfiles[i])
		free(node->outfiles[i++]);
	if (node && node->outfiles)
		free(node->outfiles);
	if (node && node->out_flags)
		free(node->out_flags);
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
