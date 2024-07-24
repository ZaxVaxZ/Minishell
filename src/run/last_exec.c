/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:21:47 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/24 16:28:49 by ehammoud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_bool	execute(t_main *m, t_cmd *cmd, t_exec *exec)
{
	char	*com;
	char	**tmp;

	if (!cmd->params || !cmd->params[0])
		return (True);
	tmp = to_char_arr(&m->env);
	if (!tmp)
		return (False);
	com = search_path(&m->env, cmd);
	if (!com || execve(com, cmd->params, tmp) == -1)
	{
		if (com)
			free(com);
		if (tmp)
			ft_freeup(tmp);
		write_error(cmd->params[0]);
		write_error(": command not found\n");
		exec->last_status = 127;
	}
	return (False);
}

int	init_exec(t_exec *exec, t_cmd **cmds)
{
	exec->std_in = dup(STDIN_FILENO);
	if (exec->std_in == -1)
	{
		perror(NULL);
		return (-1);
	}
	exec->std_out = dup(STDOUT_FILENO);
	if (exec->std_out == -1)
	{
		perror(NULL);
		return (-1);
	}
	exec->overall_status = 0;
	exec->status_depth = 0;
	exec->last_status = 0;
	exec->curr_depth = 0;
	exec->last_pid = -1;
	exec->cmd_head = cmds;
	exec->ret = 0;
	exec->last_op = NON;
	exec->fds = NULL;
	return (1);
}

t_bool	exec_cmd(t_main *m, t_cmd **cmd, t_exec *exec, int *fds)
{
	pid_t	proc_id;

	signal(SIGINT, do_nothing);
	signal(SIGQUIT, do_nothing);
	proc_id = fork();
	if (proc_id < 0)
	{
		perror(NULL);
		return (False);
	}
	if (proc_id == CHILD_PROCESS)
		child_process(m, *cmd, exec, fds);
	parent_process(*cmd, exec, fds);
	exec->last_pid = proc_id;
	exec->last_op = (*cmd)->after;
	exec->status_depth = exec->curr_depth;
	return (True);
}

t_bool	handle_cmds(t_main *m, t_cmd **cmd, t_exec *exec)
{
	int	fds[2];
	int	handle;

	handle = exec_type(exec, cmd);
	fds[READEND] = -1;
	fds[WRITEEND] = -1;
	if (!*cmd || handle == DO_NOT_EXECUTE)
		return (True);
	if (!pipe_and_set_fds(exec, fds, cmd))
		return (False);
	if ((*cmd)->heredoc && (*cmd)->heredoc_passed == 0)
		return (close_heredoc_failed(m, cmd, exec, fds));
	exec->ret = resolve_builtin(m, *cmd, exec, False);
	return (deal_with_ret(m, cmd, exec, fds));
}

int	execute_commands(t_main *m)
{
	t_exec	exec;
	t_cmd	*cmd;

	cmd = m->cmds;
	if (init_exec(&exec, &m->cmds) == -1)
		return (-1);
	exec.env = &m->env;
	exec.exit_status = &m->status;
	if (open_heredocs(m, &exec, cmd) == False)
		return (False);
	while (cmd)
	{
		exec.curr_depth -= (cmd->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (!handle_cmds(m, &cmd, &exec))
			break ;
		if (cmd)
			cmd = cmd->next;
	}
	if (post_command(m, &exec) == -1)
		return (-1);
	return (exec.ret);
}

/*
end of beginning - djo
tears in the rain - song
blame it on me - post malone live
take what you want - post malone
sirens - travis scott

movie clip:
parasyte: the grey
*/
