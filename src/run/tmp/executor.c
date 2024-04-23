/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehammoud <ehammoud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/22 16:16:20 by ehammoud         ###   ########.fr       */
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

t_bool	close_fds(int *fds)
{
	if (fds[0] != -1)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
	fds[0] = -1;
	fds[1] = -1;
	return (False);
}

int	execute_commands(t_env **env, t_cmd **cmd, int *status)
{
	t_exec	exec;
	t_cmd	*tmp;
	int temp_stdin;

	exec.cmd_head = *cmd;
	exec.overall_status = 0;
	exec.status_depth = 0;
	exec.last_status = 0;
	exec.curr_depth = 0;
	exec.last_pid = -1;
	exec.ret = 0;
	tmp = *cmd;
	temp_stdin = dup(STDIN_FILENO);
	while (tmp)
	{
		exec.curr_depth -= (tmp->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (tmp->rep != RP)
		{
			if (!handle_cmds(env, &tmp, &exec))
				break ;
			if (tmp && tmp->rep == RP)
				continue ;
		}
		if (tmp)
			tmp = tmp->next;
	}
	wait_for_children(&exec);
	if (dup_and_check(temp_stdin, STDIN_FILENO, &exec) == -1)
		return (exec.ret);
	if (exec.ret != -5)
		*status = exec.last_status;
	return (exec.ret);
}

//void	exec_parent(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
//{
//	if (cmd->after == Op_pipe || exec->last_op == Op_pipe)
//	{
//		close(fds[1]);
//		dup2(fds[0], STDIN_FILENO);
//		close(fds[0]);
//	}
//	// if (fds[1] != -1)
//	// 	close(fds[1]);
//	// if (exec->last_op == PIPE_OP && fds[0] != -1 && !cmd->input
//	// 	&& dup2(fds[0], STDIN_FILENO) < 0)
//	// 	cmd->status = EXIT_FAILURE * (!cmd->status);
//	// if (fds[0] != -1)
//	// 	close(fds[0]);
//	// if (!cmd->status && cmd->input)
//	// 	cmd->in_fd = open(cmd->input, O_RDONLY);
//	// if (!cmd->status && cmd->input
//	// 	&& (cmd->in_fd < 0 || dup2(cmd->in_fd, STDIN_FILENO) < 0))
//	// {
//	// 	perror(cmd->input);
//	// 	cmd->status = 127 * (!cmd->status);
//	// }
//}

//void	exec_child(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
//{
//	int	ret;

//	if (cmd->input && !cmd->heredoc)
//	{
//		cmd->in_fd = open(cmd->input, O_RDONLY, 0644);
//		if (cmd->in_fd == -1)
//		{
//			write_error("Cant open infile\n");
//			exit(EXIT_FAILURE);
//		}
//		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
//		{
//			write_error("Infile Dup error\n");
//			exit(EXIT_FAILURE);
//		}
//	}
//	else if (cmd->heredoc)
//	{
//		pipe(fds);
//		here_doc(env, cmd, exec, fds);
//		here_doc_parent(env, cmd, exec, fds);
//	}
//	if (cmd->outfile_cnt)
//	{
//		close(fds[0]);
//		close(fds[1]);
//		int i = 0;
//		while (i < cmd->outfile_cnt)
//		{
//			if (cmd->out_flags[i])
//				cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
//			else
//				cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
//			if (cmd->out_fd < 0)
//				perror(cmd->outfiles[cmd->outfile_cnt - 1]);
//			printf("%s, %d\n", cmd->outfiles[i], cmd->out_fd);
//			if (i < cmd->outfile_cnt - 1)
//				close(cmd->out_fd);
//			else if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
//			{
//				perror(cmd->outfiles[cmd->outfile_cnt - 1]);
//				exit(EXIT_FAILURE);
//			}
//			i++;
//		}
//	}
//	if (exec->last_op == PIPE_OP || cmd->after == Op_pipe)
//	{
//		close(fds[0]);
//		dup2(fds[1], STDOUT_FILENO);
//		close(fds[1]);
//	}
//	ret = resolve_builtin(env, cmd, exec, True);
//	if (ret == 1)
//		exit (0);
//	else if (ret == -1)
//		exit (1);
//	else if (execute(env, cmd) == False)
//		exit(EXIT_FAILURE);
//	exit(cmd->status);
//}
