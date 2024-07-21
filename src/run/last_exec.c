/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:21:47 by ehammoud          #+#    #+#             */
/*   Updated: 2024/07/21 21:04:11 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	do_nothing(int sig)
{
	if (sig == SIGINT)
		return ;
}

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
	if ((*cmd)->before == PIPE_OP || (*cmd)->after == PIPE_OP)
	{
		if (pipe_and_check(fds, exec) == -1)
			return (False);
		fprintf(stderr, "READEND: %d\n", fds[READEND]);
		fprintf(stderr, "WRITEEND: %d\n", fds[WRITEEND]);
		exec->fds = fds;
	}
	if (heredoc_loop(m, *cmd, exec, &m->env) == False)
	{
		if (fds[READEND] > 0)
		{
			fprintf(stderr, "Closing readend due to heredoc returning fail\n");
			close_and_check(fds[READEND], exec);
		}
		if (fds[WRITEEND] > 0)
		{
			fprintf(stderr, "Closing writeend due to writeend returning fail\n");
			close_and_check(fds[WRITEEND], exec);
		}
		return (False);
	}
	exec->ret = resolve_builtin(m, *cmd, exec, False);
	if (exec->ret == -5)
		return (False);
	else if (exec->ret == 2)
	{
		exec->last_status = EXIT_FAILURE;
		return (True);
	}
	if (exec->ret < 0)
	{
		exec->last_status = EXIT_FAILURE;
		return (False);
	}
	else if (exec->ret == 1)
	{
		if ((*cmd)->in_fd)
			if (close_and_check((*cmd)->in_fd, exec) == -1)
				return (-1);
		exec->last_status = SUCCESS;
		exec->last_op = (*cmd)->after;
		exec->status_depth = exec->curr_depth;
	}
	else if (exec_cmd(m, cmd, exec, fds) == False)
		return (False);
	return (True);
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
	while (cmd)
	{
		exec.curr_depth -= (cmd->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (cmd->rep != RP)
		{
			if (!handle_cmds(m, &cmd, &exec))
				break ;
			if (cmd && cmd->rep == RP)
				continue ;
		}
		if (cmd)
			cmd = cmd->next;
	}
	if (dup_and_check(exec.std_in, STDIN_FILENO, &exec) == -1)
			return (-1);
	if (dup_and_check(exec.std_out, STDOUT_FILENO, &exec) == -1)
		return (-1);
	if (close_and_check(exec.std_in, &exec) == -1)
		return (-1);
	if (close_and_check(exec.std_out, &exec) == -1)
		return (-1);
	if (wait_for_children(&exec) == -1)
		return (-1);
	if (exec.ret != -5)
		m->status = exec.last_status;
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
