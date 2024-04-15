/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 05:55:43 by codespace         #+#    #+#             */
/*   Updated: 2024/04/15 08:30:22 by marvin           ###   ########.fr       */
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

t_bool	redirect(t_cmd *cmd)
{
	int	i;

	i = -1;
	if (cmd->input)
	{
		cmd->in_fd = open(cmd->input, O_RDONLY);
		if (cmd->in_fd == -1)
			return (write_error("Couldn't open infile\n"));
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (write_error("Couldn't dup STDIN with infile\n"));
		close(cmd->in_fd);
	}
	while (++i < cmd->outfile_cnt)
	{
		if (cmd->out_flags[i] == 1)
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND, 0644);
		else if (!cmd->out_flags[i])
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC, 0644);
		if (cmd->out_fd == -1)
			return (write_error("Couldn't open outfile\n"));
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (write_error("Couldn't dup outfiles\n"));
		close(cmd->out_fd);
	}
	return (True);
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

	if (!cmd->params || !get_var(*env, "PATH"))
		return (NULL);
	paths = ft_split(get_var(*env, "PATH"), ':');
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
		if (exec->last_status == SUCCESS && exec->last_op == OR_OP)
			return (False);
		if (exec->last_status != SUCCESS && exec->last_op == AND_OP)
			return (False);
		return (True);
	}
	else
	{
		if (exec->last_status == SUCCESS && exec->last_op == OR_OP)
			ret = False;
		if (exec->last_status != SUCCESS && exec->last_op == AND_OP)
			ret = False;
		return (ret);
	}
}

int	exec_type(t_exec *exec, t_cmd **cmd)
{
	t_bool	ret;

	if ((*cmd)->rep == RP)
		return (DO_NOT_EXECUTE);
	if (exec->last_op == OR_OP || exec->last_op == AND_OP || exec->last_op == SEMICOLON)
		wait_for_children(exec);
	while (*cmd)
	{
		exec->curr_depth += ((*cmd)->rep == LP);
		ret = should_exec(exec, *cmd);
		if (!(*cmd)->rep && (*cmd)->next && (*cmd)->next->rep == RP)
			exec->last_op = after_to_op(*cmd);
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
