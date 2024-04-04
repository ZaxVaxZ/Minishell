/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 22:53:20 by marvin            #+#    #+#             */
/*   Updated: 2024/04/04 05:18:56 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "../tmp/tmp_utils.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

t_bool	clean_whitespace(t_queue *q)
{
	char	*tmp;
	t_queue	*head;

	head = q;
	while (q && q->next)
	{
		if ((q->type == Assign || q->type == Op_redir)
			&& q->next && q->next->type == Word)
			q->next->type = Name;
		if (q->type == Assign)
		{
			tmp = q->s;
			q->s = ft_substr(q->s, 0, ft_strlen(q->s) - 1);
			free(tmp);
			if (!q->s)
				return (free_queue(&head));
		}
		if (q->next->type == Whitespace)
			delete_next(&q);
		q = q->next;
	}
	return (True);
}

t_bool	redirect(t_cmd *cmd)
{
	int	i;

	i = -1;
	if (cmd->input)
	{
		cmd->in_fd = open(cmd->input, O_RDONLY);
		if (cmd->in_fd == -1)
			return (write_error("Couldn't open infile\n"));
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (write_error("Couldn't dup STDIN with infile\n"));
		close(cmd->in_fd);
	}
	while (++i < cmd->outfile_cnt)
	{
		if (cmd->out_flags[i] == 1)
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND | O_RDWR, 0644);
		else if (!cmd->out_flags[i])
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (cmd->out_fd == -1)
			return (write_error("Couldn't open outfile\n"));
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (write_error("Couldn't dup outfiles\n"));
		close(cmd->out_fd);
	}
	return (True);
}

// /// @brief searches for command in the env path variable
// /// @param env the environment with the path variable
// /// @param cmd the command
// /// @return returns path if found, else NULL
char	*search_path(t_env **env, t_cmd *cmd)
{
 	char	*path;
 	char	**paths;
 	char	*com;
 	int		i;

	if (!cmd->params)
		return (NULL);
 	path = get_var(*env, "PATH");
 	if (!path)
 		return (NULL);
 	paths = ft_split(path, ':');
 	if (!paths)
 	{
		write_error("Couldn't find path\n");
 		return (NULL);
 	}
 	i = -1;
 	while (paths[++i])
 	{
 		com = ft_strjoin_chr(paths[i], '/', cmd->params[0]);
 		if (!com)
 			return (NULL);
 		if (!access(com, X_OK | F_OK))
 			return (com);
 	}
 	return (NULL);
}

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

t_bool	handle_cmd(t_env **env, t_cmd *cmd, t_exec *exec, int *status)
{
	int	ret;

	if (cmd->rep == LP || cmd->rep == RP)
		return (True);
	ret = resolve_builtin(env, cmd);
	if (ret < 0)
		return (False);
	else if (ret == 1)
		exec->last_status = 0;
	else
	{
		//id = fork();
		//if (id == 0)
		//{
		//	if (execute(env, cmd) == False)
		//		exit(EXIT_FAILURE);
		//}
		//else
		//	wait(&cmd->status);
	}
	//if (cmd->after == Op_logic && !cmd->or_op)
	//{
	//	if (WEXITSTATUS(cmd->status))
	//	{
	//		if (cmd->next)
	//			cmd = cmd->next;
	//	}
	//}
	//else if (cmd->after == Op_logic && cmd->or_op)
	//{
	//	if (WEXITSTATUS(cmd->status) == 0)
	//	{
	//		if (cmd->next->rep == RP)
	//		{
	//			if (cmd->next)
	//				cmd = cmd->next;
	//		}
	//		if (cmd->next)
	//			cmd = cmd->next;
	//	}
	//}
	return (True);
}

int	execute_command(t_env **env, t_cmd **cmd, int *status)
{
	t_cmd	*tmp;
	t_exec	exec;

	exec.curr_depth = 0;
	exec.status_depth = 0;
	exec.overall_status = 0;
	tmp = *cmd;
	while (tmp)
	{
		exec.curr_depth += (tmp->rep == LP);
		exec.curr_depth -= (tmp->rep == RP);
		exec.status_depth -= (exec.curr_depth < exec.status_depth);
		if (!handle_cmd(env, tmp, &exec, status))
			break ;
		tmp = tmp->next;
	}
	free_cmd(cmd);
	*status = exec.overall_status;
	return (ret);
}
