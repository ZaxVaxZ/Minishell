/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uwubuntu <uwubuntu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/07 01:54:16 by uwubuntu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "get_next_line.h"

t_bool	here_doc(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	char	*line;

	while (1)
	{
		ft_printf("> ");
		line = get_next_line(STDIN_FILENO);
		if (!ft_strncmp(line, cmd->input, ft_strlen(line) - 1)
			&& ft_strlen(line) - 1 == ft_strlen(cmd->input))
			break ;
		free(line);
	}
	if (line)
		free(line);
}

t_bool	execute(t_env **env, t_cmd *cmd)
{
	int		i;
	char	*com;
	char	**tmp;

	if (!cmd->params || !cmd->params[0])
		return (True);
	// if (cmd->input || (cmd->outfile_cnt && cmd->outfiles && cmd->outfiles[0]))
	// {
	// 	if (redirect(cmd) == False)
	// 		return (False);
	// }
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
		cmd->status = 127;
	}
	return (True);
}

void	exec_parent(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	if (cmd->after == Op_pipe || exec->last_op == Op_pipe)
	{
		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		close(fds[0]);
	}
	// if (fds[1] != -1)
	// 	close(fds[1]);
	// if (exec->last_op == PIPE_OP && fds[0] != -1 && !cmd->input
	// 	&& dup2(fds[0], STDIN_FILENO) < 0)
	// 	cmd->status = EXIT_FAILURE * (!cmd->status);
	// if (fds[0] != -1)
	// 	close(fds[0]);
	// if (!cmd->status && cmd->input)
	// 	cmd->in_fd = open(cmd->input, O_RDONLY);
	// if (!cmd->status && cmd->input
	// 	&& (cmd->in_fd < 0 || dup2(cmd->in_fd, STDIN_FILENO) < 0))
	// {
	// 	perror(cmd->input);
	// 	cmd->status = 127 * (!cmd->status);
	// }
}

void	exec_child(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	if (cmd->input && !cmd->heredoc)
	{
		ft_printf("Opening infile\n");
		cmd->in_fd = open(cmd->input, O_RDONLY, 0644);
		if (cmd->in_fd == -1)
		{
			write_error("Cant open infile\n");
			exit(EXIT_FAILURE);
		}
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			write_error("Infile Dup error\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (cmd->heredoc)
	{
		here_doc(env, cmd, exec, fds);
	}
	if (cmd->outfile_cnt)
	{
		int i = 0;
		while (i < cmd->outfile_cnt)
		{
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND, 0644);
			close(fds[0]);
			close(fds[1]);
			if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			{
				write_error("Couldn't dup outfile\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}
	}
	if (exec->last_op == Op_pipe || cmd->after == Op_pipe)
	{
		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		close(fds[1]);
	}
	if (execute(env, cmd) == False)
		exit(EXIT_FAILURE);
	// int	i;
	// int	exit_code;

	// exit_code = 0;
	// if (fds[0] != -1)
	// 	close(fds[0]);
	// if (cmd->after == Op_pipe && fds[1] != -1 && !cmd->outfile_cnt
	// 	&& dup2(fds[1], STDOUT_FILENO) < 0 && !cmd->status)
	// 	exit_code = EXIT_FAILURE * (!exit_code);
	// if (fds[1] != -1)
	// 	close(fds[1]);
	// i = -1;
	// while (++i < cmd->outfile_cnt)
	// {
	// 	if (!cmd->out_flags[i])
	// 		cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_WRONLY, 0644);
	// 	else
	// 		cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC, 0644);
	// 	exit_code += EXIT_FAILURE * (!exit_code && cmd->out_fd < 0);
	// 	if (cmd->out_fd < 0)
	// 		perror(cmd->outfiles[i]);
	// }
	// if (cmd->outfile_cnt && dup2(cmd->out_fd, STDOUT_FILENO) < 0)
	// {
	// 	perror(cmd->outfiles[cmd->outfile_cnt - 1]);
	// 	exit_code += EXIT_FAILURE * (!exit_code);
	// }
	// if (!exit_code && !execute(env, cmd))
	// 	exit_code = EXIT_FAILURE;
	// if (!exit_code)
	// 	exit(cmd->status);
	// exit(exit_code);
}

int	after_to_op(t_cmd *cmd)
{
	if (cmd->after == Op_logic && cmd->or_op)
		return (OR_OP);
	if (cmd->after == Op_logic)
		return (AND_OP);
	if (cmd->after == Op_pipe)
		return (PIPE_OP);
	if (cmd->after == Semicolon)
		return (SEMICOLON);
	return (NON);
}

t_bool	execute_cmd(t_env **env, t_cmd *cmd, t_exec *exec, int *fds)
{
	int		i;
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (False);
	if (pid == CHILD_PROCESS)
		exec_child(env, cmd, exec, fds);
	exec_parent(env, cmd, exec, fds);
	exec->last_pid = pid;
	exec->last_op = after_to_op(cmd);
	exec->last_status += cmd->status * (!exec->last_status);
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

t_bool	handle_cmd(t_env **env, t_cmd **cmd, t_exec *exec)
{
	int		handle;
	int		fds[2];

	handle = exec_type(exec, cmd);
 	if (!*cmd || handle == DO_NOT_EXECUTE)
 		return (True);
	fds[0] = -1;
	fds[1] = -1;
	if (exec->last_op == PIPE_OP || (*cmd)->after == Op_pipe)
		pipe(fds);
 	exec->ret = resolve_builtin(env, *cmd);
 	if (exec->ret < 0)
 		return (close_fds(fds));
 	else if (exec->ret == 1)
	{
		exec->last_status = 0;
		exec->last_pid = -1;
		exec->last_op = after_to_op(*cmd);
	}
	else if (!execute_cmd(env, *cmd, exec, fds))
		return (close_fds(fds));
	close_fds(fds);
 	return (True);
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
	exec.last_op = NON;
	exec.ret = 0;
	tmp = *cmd;
	temp_stdin = dup(STDIN_FILENO);
	while (tmp)
	{
		exec.curr_depth -= (tmp->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (tmp->rep != RP)
		{
			if (!handle_cmd(env, &tmp, &exec))
				break ;
			if (tmp && tmp->rep == RP)
				continue ;
		}
		if (tmp)
			tmp = tmp->next;
	}
	wait_for_children(&exec);
	dup2(temp_stdin, STDIN_FILENO);
	if (exec.ret != -5)
		*status = exec.last_status;
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