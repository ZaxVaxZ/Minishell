/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 14:21:47 by ehammoud          #+#    #+#             */
/*   Updated: 2024/04/22 20:27:14 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_bool	execute(t_env **env, t_cmd *cmd, t_exec *exec)
{
	int		i;
	char	*com;
	char	**tmp;

	if (!cmd->params || !cmd->params[0])
		return (True);
	tmp = to_char_arr(env);
	if (!tmp)
		return (False);
	com = search_path(env, cmd);
	if (!com || execve(com, cmd->params, tmp) == -1)
	{
		if (com)
			free(com);
		ft_freeup(tmp);
		write_error(cmd->params[0]);
		write_error(": command not found\n");
		exec->last_status = 127;
	}
	return (False);
}

int	init_exec(t_exec *exec, int *stand_in)
{
	*stand_in = dup(STDIN_FILENO);
	if (*stand_in == -1)
		return (-1);
	exec->overall_status = 0;
	exec->status_depth = 0;
	exec->last_status = 0;
	exec->curr_depth = 0;
	exec->last_pid = -1;
	exec->ret = 0;
	exec->last_op = NON;
	return (1);
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
	exec->last_op = (*cmd)->after;
	exec->status_depth = exec->curr_depth;
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
	{
		if (pipe_and_check(fds, exec) == -1)
			return (False);
	}
	exec->ret = resolve_builtin(env, *cmd, exec, False);
	if (exec->ret < 0)
		return (False);
	else if (exec->ret == 1)
	{
		exec->last_status = SUCCESS;
		exec->last_op = (*cmd)->after;
		exec->status_depth = exec->curr_depth;
	}
	else if (exec_cmd(env, cmd, exec, fds) == False)
		return (False);
	return (True);
}

int	execute_commands(t_env **env, t_cmd *cmd, int *status)
{
	t_exec	exec;
	int		sin;

	if (init_exec(&exec, &sin) == -1)
		return (-1);
	while (cmd)
	{
		exec.curr_depth -= (cmd->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (cmd->rep != RP)
		{
			if (!handle_cmds(env, &cmd, &exec))
				break ;
			if (cmd && cmd->rep == RP)
				continue ;
		}
		if (cmd)
			cmd = cmd->next;
	}
	if (wait_for_children(&exec, &sin) == -1)
		return (-1);
	if (exec.ret != -5)
		*status = exec.last_status;
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