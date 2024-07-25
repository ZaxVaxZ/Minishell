/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:54:31 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/25 16:29:07 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	open_ins(t_main *m, t_cmd *cmd, t_exec *exec)
{
	int	i;

	if (cmd->infile_cnt)
	{
		i = -1;
		while (++i < cmd->infile_cnt)
		{
			if (!cmd->in_flags[i]
				&& (open_and_check(&cmd->in_fd, cmd->infiles[i],
						0, exec) == -1))
			{
				m->status = 1;
				if (cmd->before == PIPE_OP || cmd->after == PIPE_OP)
				{
					close_pipes(m, exec, cmd, exec->fds);
					free_and_exit(m, -1);
				}
				return (-1);
			}
		}
	}
	return (1);
}

int	open_outs_and_in(t_main *m, t_cmd *cmd, t_exec *exec)
{
	int	i;

	if (open_ins(m, cmd, exec) == -1)
		return (-1);
	if (cmd->outfile_cnt)
	{
		i = -1;
		while (++i < cmd->outfile_cnt)
		{
			if (open_and_check(&cmd->out_fd, cmd->outfiles[i],
					cmd->out_flags[i] + 1, exec) == -1)
				return (-1);
			if (i != cmd->outfile_cnt - 1
				&& close_and_check(cmd->out_fd, exec) == -1)
				return (-1);
		}
	}
	return (1);
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

void	child_process(t_main *m, t_cmd *cmd, t_exec *exec, int *fds)
{
	close(exec->std_in);
	close(exec->std_out);
	if (open_outs_and_in(m, cmd, exec) == -1)
	{
		if (cmd->before == PIPE_OP || cmd->after == PIPE_OP)
		{
			close(fds[READEND]);
			close(fds[WRITEEND]);
		}
		free_and_exit(m, -1);
	}
	dups_and_closes(cmd, exec, &m->env, fds);
	if (resolve_builtin(m, cmd, exec, True) == 0)
		execute(m, cmd, exec);
	if (close_and_check(STDIN_FILENO, exec) == -1)
		free_and_exit(m, ERR_CLS);
	if (close_and_check(STDOUT_FILENO, exec) == -1)
		free_and_exit(m, ERR_CLS);
	free_queue(&m->q);
	free_cmd(&m->cmds);
	free_env(&m->env);
	free(m->line);
	exit(exec->last_status);
}

int	parent_process(t_cmd *cmd, t_exec *exec, int *fds)
{
	if (cmd->infile_cnt && cmd->in_fd > 0)
	{
		if (close_and_check(cmd->in_fd, exec) == -1)
			return (-1);
	}
	if (cmd->before == PIPE_OP || cmd->after == PIPE_OP)
	{
		if (close_and_check(fds[WRITEEND], exec) == -1)
			return (-1);
		if (dup_and_check(fds[READEND], STDIN_FILENO, exec) == -1)
			return (-1);
		if (close_and_check(fds[READEND], exec) == -1)
			return (-1);
	}
	return (1);
}
