/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:31 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/16 14:04:33 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	open_outs_and_in(t_cmd *cmd, t_exec *exec)
{
	int	i;

	if (cmd->infile_cnt)
	{
		i = -1;
		while (++i < cmd->infile_cnt)
		{
			if (!cmd->in_flags[i] && (open_and_check(&cmd->in_fd, cmd->infiles[i], 0, exec) == -1))
					return (-1);
			if (i != cmd->infile_cnt - 1 && !cmd->in_flags[i] && close_and_check(cmd->in_fd, exec) == -1)
				return (-1);
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

static void	child_free_and_exit(t_env **env, t_exec *exec, int status)
{
	free_env(env);
	free_cmd(exec->cmd_head);
	exit(status);
}

static void	dups_and_closes(t_cmd *cmd, t_exec *exec, t_env **env, int *fds)
{
	if (cmd->infile_cnt)
	{
		if (dup_and_check(cmd->in_fd, STDIN_FILENO, exec) == -1)
			child_free_and_exit(env, exec, EXIT_FAILURE);
		if (close_and_check(cmd->in_fd, exec) == -1)
			child_free_and_exit(env, exec, EXIT_FAILURE);
	}
	if (cmd->outfile_cnt)
	{
		if (dup_and_check(cmd->out_fd, STDOUT_FILENO, exec) == -1)
			child_free_and_exit(env, exec, EXIT_FAILURE);
		if (close_and_check(cmd->out_fd, exec) == -1)
			child_free_and_exit(env, exec, EXIT_FAILURE);
	}
	if (cmd->after == PIPE_OP && !cmd->outfile_cnt)
	{
		if (close_and_check(fds[0], exec) == -1)
			child_free_and_exit(env, exec, exec->last_status);
		if (dup_and_check(fds[1], STDOUT_FILENO, exec) == -1)
			child_free_and_exit(env, exec, exec->last_status);
		if (close_and_check(fds[1], exec) == -1)
			child_free_and_exit(env, exec, exec->last_status);
	}
}

void	child_sig(int sig)
{
	(void)sig;
	return ;
}

/// @brief this will execute the command and open/close pipes accordingly
/// @param cmd the cmd node
/// @param exec the exec struct
/// @param fds the piped fds
/// @return will exit depending on whether the commaned succeeded or failed
void	child_process(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	int	ret;

	close(exec->std_in);
	close(exec->std_out);
	if (open_outs_and_in(cmd, exec) == -1)
		child_free_and_exit(env, exec, exec->last_status);
	dups_and_closes(cmd, exec, env, fds);
	ret = resolve_builtin(env, cmd, exec, True);
	if (ret == 0)
		execute(env, cmd, exec);
	child_free_and_exit(env, exec, exec->last_status);
}

int	parent_process(t_cmd *cmd, t_exec *exec, int *fds)
{
	if (cmd->in_fd)
	{
		if (close_and_check(cmd->in_fd, exec) == -1)
			return (-1);
	}
	if (cmd->before == PIPE_OP || cmd->after == PIPE_OP)
	{
		if (close_and_check(fds[WRITEEND], exec) == -1)
			return (-1);
		if (cmd->after == PIPE_OP)
			if (dup_and_check(fds[READEND], STDIN_FILENO, exec) == -1)
				return (-1);
		if (close_and_check(fds[READEND], exec) == -1)
			return (-1);
	}
	return (1);
}
