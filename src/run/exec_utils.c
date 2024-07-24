/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:28:10 by pipolint          #+#    #+#             */
/*   Updated: 2024/07/24 17:20:21 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_bool	close_heredoc_failed(t_main *m, t_cmd **cmd, t_exec *exec, int *fds)
{
	if ((*cmd)->heredoc && (*cmd)->heredoc_passed == 0)
	{
		if ((*cmd)->in_fd > 0)
		{
			if (close_and_check((*cmd)->in_fd, exec) == False)
				free_and_exit(m, ERR_CLS);
		}
		if (fds[READEND] > 0)
		{
			if (close_and_check(fds[READEND], exec) == False)
				free_and_exit(m, ERR_CLS);
		}
		if (fds[WRITEEND] > 0)
		{
			if (close_and_check(fds[WRITEEND], exec) == False)
				free_and_exit(m, ERR_CLS);	
		}
	}
	return (False);
}

int	pipe_and_set_fds(t_exec *exec, int *fds, t_cmd **cmd)
{
	if ((*cmd)->before == PIPE_OP || (*cmd)->after == PIPE_OP)
	{
		if (pipe_and_check(fds, exec) == False)
			return (False);
		exec->fds = fds;
	}
	return (True);
}

t_bool	deal_with_ret(t_main *m, t_cmd **cmd, t_exec *exec, int *fds)
{
	if (exec->ret == -5)
		return (True);
	else if (exec->ret == 2 || exec->ret < 0)
	{
		exec->last_status = EXIT_FAILURE;
		if (exec->ret < 0)
			return (False);
		return (True);
	}
	else if (exec->ret == 1)
	{
		if ((*cmd)->in_fd)
			if (close_and_check((*cmd)->in_fd, exec) == -1)
				return (False);
		exec->last_status = SUCCESS;
		exec->last_op = (*cmd)->after;
		exec->status_depth = exec->curr_depth;
	}
	else if (exec_cmd(m, cmd, exec, fds) == False)
		return (False);
	return (True);
}

t_bool	open_heredocs(t_main *m, t_exec *exec, t_cmd *heredoc)
{
	while (heredoc)
	{
		if (heredoc_loop(m, heredoc, exec, &m->env) == False)
		{
			if (heredoc->in_fd > 0)
			{
				if (close_and_check(heredoc->in_fd, exec) == False)
					break ;
			}
			heredoc->heredoc_passed = 0;
			break ;
		}
		heredoc = heredoc->next;
	}
	return (True);
}

int	post_command(t_main *m, t_exec *exec)
{
	if (dup_and_check(exec->std_in, STDIN_FILENO, exec) == -1)
			return (-1);
	if (dup_and_check(exec->std_out, STDOUT_FILENO, exec) == -1)
		return (-1);
	if (close_and_check(exec->std_in, exec) == -1)
		return (-1);
	if (close_and_check(exec->std_out, exec) == -1)
		return (-1);
	if (wait_for_children(exec) == -1)
		return (-1);
	if (exec->ret != -5)
		m->status = exec->last_status;
	return (1);
}
