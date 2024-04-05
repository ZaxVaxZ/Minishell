/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 05:55:43 by codespace         #+#    #+#             */
/*   Updated: 2024/04/05 07:09:27 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND, 0644);
		else if (!cmd->out_flags[i])
			cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC, 0644);
		if (cmd->out_fd == -1)
			return (write_error("Couldn't open outfile\n"));
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (write_error("Couldn't dup outfiles\n"));
		close(cmd->out_fd);
	}
	return (True);
}

/// @brief searches for command in the env path variable
/// @param env the environment with the path variable
/// @param cmd the command
/// @return returns path if found, else NULL
char	*search_path(t_env **env, t_cmd *cmd)
{
	char	**paths;
	char	*com;
	int		i;

	if (!cmd->params)
		return (NULL);
	paths = ft_split(get_var(*env, "PATH"), ':');
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

t_bool	should_exec(t_exec *exec, t_cmd *cmd)
{
	t_bool	ret;

	ret = True;
	if (exec->curr_depth != exec->status_depth)
	{
		if (exec->last_status == SUCCESS && exec->last_op == OR_OP)
			return (False);
		if (exec->last_status != SUCCESS && exec->last_op == AND_OP)
			return (False);
		return (True);
	}
	else
	{
		if (exec->last_status == SUCCESS && exec->last_op == OR_OP)
			ret = False;
		if (exec->last_status != SUCCESS && exec->last_op == AND_OP)
			ret = False;
		exec->last_op = cmd->after;
		return (ret);
	}
}

int	exec_type(t_exec *exec, t_cmd **cmd)
{
	if ((*cmd)->rep == RP)
		return (DO_NOT_EXECUTE);
	while (*cmd)
	{
		exec->curr_depth += ((*cmd)->rep == LP);
		if ((*cmd)->rep == LP || !should_exec(exec, *cmd))
			(*cmd) = (*cmd)->next;
		else
			break ;
	}
	if (exec->last_op == NON || exec->last_op == PIPE_OP)
		return (IMMEDIATE_EXEC);
	if (exec->last_op == SEMICOLON)
		return (WAIT_THEN_EXEC);
	if (exec->last_op != OR_OP && exec->last_op != AND_OP)
		return (True);
}
