/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:48:58 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/30 16:36:19 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	open_infiles(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	int	i;

	i = -1;
	if (cmd->infile_cnt)
	{
		while (cmd->infiles[++i])
		{
			if (!cmd->in_flags[i])
			{
				if (open_and_check(&cmd->in_fd, cmd->infiles[i], 0, exec) == -1)
					return (-1);
			}
			else if (cmd->in_flags[i])
				heredoc(cmd, exec, fds, i);
			if (!cmd->in_flags[i] && i < cmd->infile_cnt - 1)
			{
				if (close_and_check(cmd->in_fd, exec) == -1)
					return (-1);
			}
		}
		if (!cmd->in_flags[i] && (dup_and_check(cmd->in_fd, STDIN_FILENO, exec) == False))
			return (-1);
		if (!cmd->in_flags[i] && (close_and_check(cmd->in_fd, exec) == False))
			return (-1);
	}
	return (1);
}

int	open_outfiles(t_cmd *cmd, t_exec *exec)
{
	int	i;

	i = -1;
	if (cmd->outfile_cnt)
	{
		while (++i < cmd->outfile_cnt)
		{
			if (open_and_check(&cmd->out_fd, cmd->outfiles[i], cmd->out_flags[i] + 1, exec) == -1)
				return (-1);
			if (i < cmd->outfile_cnt - 1)
				close_and_check(cmd->out_fd, exec);
		}
		if (dup_and_check(cmd->out_fd, STDOUT_FILENO, exec) == False)
			return (-1);
		return (close_and_check(cmd->out_fd, exec));
	}
	return (0);
}

t_bool	exec_cmd(t_env **env, t_cmd **cmd, t_exec *exec, int *fds)
{
	pid_t	proc_id;

	proc_id = fork();
	if (proc_id < 0)
	{
		perror(NULL);
		return (False);
	}
	if (proc_id == CHILD_PROCESS)
		child_process(env, *cmd, exec, fds);
	parent_process(*cmd, exec, fds);
	exec->last_pid = proc_id;
	return (True);
}

t_bool	handle_cmds(t_env **env, t_cmd **cmd, t_exec *exec)
{
	int	fds[2];
	int	handle;

	handle = exec_type(exec, cmd);
	if (!*cmd || handle == DO_NOT_EXECUTE)
		return (True);
	if ((*cmd)->before == PIPE_OP || (*cmd)->after == PIPE_OP)
		if (pipe_and_check(fds, exec) == -1)
			return (False);
	if (((*cmd)->before != PIPE_OP && (*cmd)->after != PIPE_OP)
		&& open_outs_and_in(*cmd, exec) == -1)
			return (False);
	exec->ret = resolve_builtin(env, *cmd, exec, False);
	if (exec->ret < 0)
		return (False);
	else if (exec->ret == 1)
		exec->last_status = SUCCESS;
	else
	{
		if (exec_cmd(env, cmd, exec, fds) == False)
			return (False);
	}
	return (True);
}

int	execute_cmds(t_env **env, t_cmd **cmds, int *status)
{
	t_exec	e;
	t_cmd	*curr_cmd;
	int		stdin;

	e.curr_depth = 0;
	e.last_status = 0;
	e.status_depth = 0;
	e.last_pid = -1;
	curr_cmd = (*cmds);
	stdin = dup(STDIN_FILENO);
	while (curr_cmd)
	{
		e.curr_depth += (curr_cmd->rep == LP);
		e.curr_depth -= (curr_cmd->rep == RP);
		e.status_depth -= (e.curr_depth < e.status_depth);
		if (handle_cmds(env, &curr_cmd, &e) == False)
			break ;
	}
	wait_for_children(&e, );
	if (e.ret != -5)
		*status = e.last_status;
	if (dup_and_check(stdin, STDIN_FILENO, &e) == False)
		return (EXIT_FAILURE);
	return (e.ret);
}
