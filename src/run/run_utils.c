/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 05:55:43 by codespace         #+#    #+#             */
/*   Updated: 2024/04/18 15:31:54 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	wait_for_children(t_exec *exec)
{
	int		status;
	pid_t	child;

	child = 0;
	while (child != -1)
	{
		child = wait(&status);
		if (exec->last_pid != -1 && (int)child == exec->last_pid)
		{
			exec->last_status = WEXITSTATUS(status);
			exec->status_depth = exec->curr_depth;
		}
	}
}

/// @brief searches for command in the env path variable
/// @param env the environment with the path variable
/// @param cmd the command
/// @return returns path if found, else NULL
char	*search_path(t_env **env, t_cmd *cmd)
{
	char	**paths;
	char	*com;
	int		i;

	if (!cmd->params || !cmd->params[0])
		return (NULL);
	if (!get_var(*env, "PATH"))
		return (cmd->params[0]);
	paths = ft_split(get_var(*env, "PATH"), ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths && paths[++i])
	{
		com = ft_strjoin_chr(paths[i], '/', cmd->params[0]);
		if (!com)
			return ((char *)ft_freeup(paths));
		if (!access(com, X_OK | F_OK))
		{
			ft_freeup(paths);
			return (com);
		}
		free(com);
	}
	return ((char *)ft_freeup(paths));
}

t_bool	should_exec(t_exec *exec, t_cmd *cmd)
{
	t_bool	ret;

	if (cmd->rep == RP)
		return (True);
	ret = True;
	if (exec->curr_depth != exec->status_depth)
	{
		if (exec->last_status == SUCCESS && cmd->before == OR_OP)
			return (False);
		if (exec->last_status != SUCCESS && cmd->before == AND_OP)
			return (False);
		return (True);
	}
	else
	{
		if (exec->last_status == SUCCESS && cmd->before == OR_OP)
			ret = False;
		if (exec->last_status != SUCCESS && cmd->before == AND_OP)
			ret = False;
		return (ret);
	}
}

int	exec_type(t_exec *exec, t_cmd **cmd)
{
	t_bool	ret;

	if ((*cmd)->rep == RP || (*cmd)->rep == LP)
		return (DO_NOT_EXECUTE);
	if ((*cmd)->before == OR_OP || (*cmd)->before == AND_OP || (*cmd)->before == SEMICOLON)
		wait_for_children(exec);
	while (*cmd)
	{
		exec->curr_depth += ((*cmd)->rep == LP);
		ret = should_exec(exec, *cmd);
		if ((*cmd)->rep == LP || !ret)	
			(*cmd) = (*cmd)->next;	
		else
			break ;
	}
	if (!*cmd || (*cmd)->rep == RP)
		return (DO_NOT_EXECUTE);	
	return (IMMEDIATE_EXEC);
	// if (exec->last_op == NON || exec->last_op == PIPE_OP)
	// 	return (IMMEDIATE_EXEC);
	// if (exec->last_op == SEMICOLON)
	// 	return (WAIT_THEN_EXEC);
	// if (exec->last_op == OR_OP || exec->last_op == AND_OP)
	// 	return (WAIT_THEN_EXEC);
}
