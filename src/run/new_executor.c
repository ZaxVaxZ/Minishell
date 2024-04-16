/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:48:58 by pipolint          #+#    #+#             */
/*   Updated: 2024/04/15 18:49:08 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	open_infiles(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	if (cmd->input)
	{
		if (!cmd->heredoc)
		{
			cmd->in_fd = open(cmd->input, O_RDONLY);
			if (cmd->in_fd < 0)
			{
				perror(cmd->input);
				return (-1);
			}
			if (dup_and_check(cmd->in_fd, STDIN_FILENO) == False)
				return (False);
			close(cmd->in_fd);
			return (1);
		}
		else
		{
			if (heredoc(env, cmd, exec, fds) == False)
				return (-1);
		}
	}
	return (0);
}

int	open_outfiles(t_cmd *cmd)
{
	int	i;

	i = -1;
	if (cmd->outfile_cnt)
	{
		while (++i < cmd->outfile_cnt)
		{
			if (cmd->out_flags[i])
				cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
			else
				cmd->out_fd = open(cmd->outfiles[i], O_CREAT| O_TRUNC | O_WRONLY, 0644);
			if (cmd->out_fd < 0)
			{
				perror(cmd->outfiles[i]);
				return (-1);
			}
			if (i < cmd->outfile_cnt - 1)
				close(cmd->out_fd);
		}
		if (dup_and_check(cmd->out_fd, STDOUT_FILENO) == False)
			return (False);
		close(cmd->out_fd);
		return (1);
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
	if (proc_id == 0)
		child_process(env, *cmd, exec, fds);
	parent_process(*cmd, fds);
	exec->last_pid = proc_id;
	exec->last_status += (*cmd)->status * (!exec->last_status);
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
		if (pipe_and_check(fds) == -1)
			return (False);
	if (open_infiles(env, *cmd, exec, fds) == -1)
		return (False);
	if (open_outfiles(*cmd) == -1)
		return (False);
	exec->ret = resolve_builtin(env, *cmd, exec, False);
	if (exec->ret < 0)
		return (False);
	else if (exec->ret == 1)
		exec->last_status = 0;
	else if (exec_cmd(env, cmd, exec, fds) == False)
		return (False);
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
	curr_cmd = (*cmds);
	stdin = dup(STDIN_FILENO);
	while (curr_cmd)
	{
		e.curr_depth += (curr_cmd->rep == LP);
		e.curr_depth -= (curr_cmd->rep == RP);
		e.status_depth -= (e.curr_depth < e.status_depth);
		if (handle_cmds(env, &curr_cmd, &e) == False)
			break ;
		if (curr_cmd && curr_cmd->rep == RP)
			continue ;
		if (curr_cmd)
			curr_cmd = curr_cmd->next;
	}
	wait_for_children(&e);
	if (e.ret != -5)
		*status = e.last_status;
	if (dup_and_check(stdin, STDIN_FILENO) == False)
		return (EXIT_FAILURE);
	return (e.ret);
}