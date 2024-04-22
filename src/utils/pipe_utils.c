/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:31 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/22 19:31:41 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	open_outs_and_in(t_cmd *cmd, t_exec *exec, int *fds)
{
	int	i;

	if (cmd->infile_cnt)
	{
		i = -1;
		while (++i < cmd->infile_cnt)
		{
			if (!cmd->in_flags[i])
			{
				if (open_and_check(&cmd->in_fd, cmd->infiles[i], 0, exec) == -1)
					return (-1);
				if (i != cmd->infile_cnt - 1 && close_and_check(cmd->in_fd, exec) == -1)
					return (-1);
			}
			//else if (cmd->in_flags[i])
			//	heredoc(cmd, exec, fds, i);	
		}
	}
	if (cmd->outfile_cnt)
	{
		i = -1;
		while (++i < cmd->outfile_cnt)
		{
			if (open_and_check(&cmd->out_fd, cmd->outfiles[i], cmd->out_flags[i] + 1, exec) == -1)
				return (-1);
			if (i != cmd->outfile_cnt - 1 && close_and_check(cmd->out_fd, exec) == -1)
				return (-1);
		}
	}
	return (1);
}

/// @brief this will execute the command and open/close pipes accordingly
/// @param cmd the cmd node
/// @param exec the exec struct
/// @param fds the piped fds
/// @return will exit depending on whether the commaned succeeded or failed
void	child_process(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	int	ret;

	if (open_outs_and_in(cmd, exec, fds) == -1)
		exit(exec->last_status);
	if (cmd->infile_cnt && !cmd->heredoc)
		if (dup_and_check(cmd->in_fd, STDIN_FILENO, exec) == -1)
			exit(EXIT_FAILURE);
	if (cmd->outfile_cnt)
		if (dup_and_check(cmd->out_fd, STDOUT_FILENO, exec) == -1)
			exit(EXIT_FAILURE);
	if (cmd->after == PIPE_OP && !cmd->outfile_cnt)
	{
		if (close_and_check(fds[0], exec) == -1)
			exit(exec->last_status);
		if (dup_and_check(fds[1], STDOUT_FILENO, exec) == -1)
			exit(exec->last_status);
		if (close_and_check(fds[1], exec) == -1)
			exit(exec->last_status);
	}
	ret = resolve_builtin(env, cmd, exec, True);
	if (ret == 0)
	{
		if (execute(env, cmd, exec) == False)
			exec->last_status = EXIT_FAILURE;
	}
	else if (ret == -1)
		exec->last_status = EXIT_FAILURE;
	else
		exec->last_status = SUCCESS;
	exit(exec->last_status);
}

int	parent_process(t_cmd *cmd, t_exec *exec, int *fds)
{
	if (cmd->before == PIPE_OP || cmd->after == PIPE_OP)
	{
		close(fds[1]);
		if (dup_and_check(fds[0], STDIN_FILENO, exec) == -1)
			return (-1);
		close(fds[0]);
	}
	return (1);
}
