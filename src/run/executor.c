/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/05 07:42:19 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_bool	execute(t_env **env, t_cmd *cmd)
{
	if (!cmd->params)
		return (True);
	if (cmd->input || (cmd->outfile_cnt && cmd->outfiles && cmd->outfiles[0]))
	{
		if (redirect(cmd) == False)
			return (False);
	}
	if (execve(search_path(env, cmd), cmd->params, to_char_arr(env)) == -1)
	{
		write_error(cmd->params[0]);
		write_error(": Command not found\n");
		return (False);
	}
	return (True);
}

t_bool	handle_cmd(t_env **env, t_cmd **cmd, t_exec *exec)
{
	int		handle;
 	int		ret;
	pid_t	id;

	handle = exec_type(exec, cmd);
 	if (handle == DO_NOT_EXECUTE)
 		return (True);
	else if (handle == WAIT_THEN_EXEC)
		wait_for_children(t_exec *exec);
 	exec->ret = resolve_builtin(env, *cmd);
 	if (exec->ret < 0)
 		return (False);
 	else if (exec->ret == 1)
	{
		exec->last_status = 0;
		exec->last_op = (*cmd)->after;
	}
	else
	{
		id = fork();
		if (id == 0)
		{
			if (execute(env, cmd) == False)
				exit(EXIT_FAILURE);
		}
		else
			wait(&cmd->status);
		exec->last_status = WEXITSTATUS(cmd->status);
		exec->status_depth = exec->curr_depth;
	}
	ft_printf("status: %d\n", exec->last_status);
 	return (True);
}

int	execute_command(t_env **env, t_cmd **cmd, int *status)
{
	t_exec	exec;
	t_cmd	*tmp;

	exec.overall_status = 0;
	exec.status_depth = 0;
	exec.curr_depth = 0;
	exec.last_op = NON;
	exec.ret = 0;
	tmp = *cmd;
	while (tmp)
	{
		exec.curr_depth -= (tmp->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (!handle_cmd(env, &tmp, &exec))
			break ;
		tmp = tmp->next;
	}
	free_cmd(cmd);
	*status = exec.overall_status;
	return (exec.ret);
}

//else
//{
//	//id = fork();
//	//if (id == 0)
//	//{
//	//	if (execute(env, cmd) == False)
//	//		exit(EXIT_FAILURE);
//	//}
//	//else
//	//	wait(&cmd->status);
//}
////if (cmd->after == Op_logic && !cmd->or_op)
////{
////	if (WEXITSTATUS(cmd->status))
////	{
////		if (cmd->next)
////			cmd = cmd->next;
////	}
////}
////else if (cmd->after == Op_logic && cmd->or_op)
////{
////	if (WEXITSTATUS(cmd->status) == 0)
////	{
////		if (cmd->next->rep == RP)
////		{
////			if (cmd->next)
////				cmd = cmd->next;
////		}
////		if (cmd->next)
////			cmd = cmd->next;
////	}
////}