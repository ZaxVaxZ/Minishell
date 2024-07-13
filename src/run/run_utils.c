/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 05:55:43 by codespace         #+#    #+#             */
/*   Updated: 2024/07/13 14:45:39 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	wait_for_children(t_exec *exec, t_cmd *cmds)
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
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
				exec->last_status = 130;
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
				exec->last_status = 131;
			exec->status_depth = exec->curr_depth;
		}
	}
	signal(SIGINT, sig_handle);
	signal(SIGQUIT, SIG_IGN);
	if (exec->last_op != AND_OP && exec->last_op != OR_OP)
	{
		if (dup_and_check(exec->std_in, STDIN_FILENO, exec) == -1)
			return (-1);
		if (dup_and_check(exec->std_out, STDOUT_FILENO, exec) == -1)
			return (-1);
		if (close_and_check(exec->std_in, exec) == -1)
			return (-1);
		if (close_and_check(exec->std_out, exec) == -1)
			return (-1);
	}
	(void)cmds;
	return (1);
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
		return (ft_strdup(cmd->params[0]));
	paths = ft_split(get_var(*env, "PATH"), ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths && paths[++i])
	{
		com = ft_strjoin_chr(paths[i], '/', cmd->params[0]);
		if (!com)
			return ((char *)ft_freeup(paths));
		if (!access(com, X_OK | F_OK) && !ft_freeup(paths))
			return (com);
		free(com);
	}
	ft_freeup(paths);
	return (ft_strdup(cmd->params[0]));
}

t_bool	should_exec(t_exec *exec, t_cmd *cmd)
{
	int	ret;

	ret = 1;
	if (cmd->rep == RP)
		return (True);
	if (exec->curr_depth != exec->status_depth)
	{
		if (exec->last_status == SUCCESS && exec->last_op == OR_OP)
			return (False);
		if (exec->last_status != SUCCESS && exec->last_op == AND_OP)
			return (False);
		return (True);
	}
	else
	{
		ret -= (exec->last_status == SUCCESS && cmd->before == OR_OP);
		ret -= (exec->last_status != SUCCESS && cmd->before == AND_OP);
		ret -= (exec->last_status == SUCCESS && exec->last_op == OR_OP
			&& (cmd->before == PIPE_OP || cmd->after == PIPE_OP));
		ret -= (exec->last_status != SUCCESS && exec->last_op == AND_OP
			&& (cmd->before == PIPE_OP || cmd->after == PIPE_OP));
		if (ret < 1 && cmd->after != PIPE_OP)
			exec->last_op = cmd->after;
		return (ret == 1);
	}
}

int	exec_type(t_exec *exec, t_cmd **cmd)
{
	if ((*cmd)->rep == RP)
		return (DO_NOT_EXECUTE);
	while ((*cmd)->rep == LP)
		(*cmd) = (*cmd)->next;
	if ((*cmd)->before == OR_OP || (*cmd)->before == AND_OP || (*cmd)->before == SEMICOLON)
		wait_for_children(exec, *cmd);
	while (*cmd)
	{
		exec->curr_depth += ((*cmd)->rep == LP);
		if ((*cmd)->rep == LP || !should_exec(exec, *cmd))
			(*cmd) = (*cmd)->next;
		else
			break ;
	}
	if (!*cmd || (*cmd)->rep == RP)
		return (DO_NOT_EXECUTE);
	return (IMMEDIATE_EXEC);
}
