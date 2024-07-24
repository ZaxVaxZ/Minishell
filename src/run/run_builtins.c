/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 02:36:00 by codespace         #+#    #+#             */
/*   Updated: 2024/07/24 21:30:49 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_bool	clean_whitespace(t_main *m)
{
	char	*tmp;
	t_queue	*q;

	q = m->q;
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
				return (free_queue(&m->q));
		}
		if (q->next->type == Whitespace)
			delete_next(&q);
		q = q->next;
	}
	return (True);
}

static int	builtin_helper(t_main *m, t_cmd *cmd, t_exec *exec)
{
	char	*cwd;

	if (!ft_strncmp(cmd->params[0], "cd", -1))
	{
		cwd = getcwd(NULL, 0);
		if (!cd(m, cwd, cmd->params[1]))
			return (free(cwd), -1);
		free(cwd);
	}
	else if (!ft_strncmp(cmd->params[0], "echo", -1))
	{
		if (!echo(m, cmd->params + 1))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "exit", -1))
	{
		if (exiting(m, cmd, cmd->params + 1, exec) == -2)
			return (-1);
		return (-5);
	}
	else
		return (0);
	return (1);
}

int	builtin_helper_v2(t_main *m, t_cmd *cmd)
{
	int	ret;

	ret = 1;
	if (!ft_strncmp(cmd->params[0], "unset", -1))
		unset(m, cmd->params + 1);
	else if (!ft_strncmp(cmd->params[0], "export", -1))
	{
		if (!export_cmd(m, cmd->params + 1))
			ret = -1;
	}
	else if (!ft_strncmp(cmd->params[0], "env", -1))
	{
		if (!print_env(m))
			ret = -1;
	}
	else if (!ft_strncmp(cmd->params[0], "pwd", -1))
	{
		if (ft_printf("%s\n", get_var(m->env, "PWD")) == -1)
			free_and_exit(m, ERR_WRT);
	}
	else
		return (0);
	return (ret);
}

int	resolve_builtin(t_main *m, t_cmd *cmd, t_exec *exec, t_bool child)
{
	int	ret;

	if ((!child && (cmd->before == PIPE_OP || cmd->after == PIPE_OP))
		|| (child && cmd->before != PIPE_OP && cmd->after != PIPE_OP)
		|| (!cmd || !cmd->params || !cmd->params[0]))
		return (0);
	if (cmd->infile_cnt || cmd->outfile_cnt)
	{
		if (open_outs_and_in(m, cmd, exec) == -1)
			return (close_pipes(m, exec, cmd, exec->fds));
	}
	if (cmd->outfile_cnt)
	{
		if (dup_and_check(cmd->out_fd, STDOUT_FILENO, exec) == -1
			|| close_and_check(cmd->out_fd, exec) == -1)
			return (-1);
	}
	if (cmd->before == PIPE_OP && cmd->after != PIPE_OP
		&& (close_and_check(exec->fds[0], exec) == -1
			|| close_and_check(exec->fds[1], exec) == -1))
		free_and_exit(m, ERR_CLS);
	ret = builtin_helper(m, cmd, exec);
	if (ret == 1 || ret < 0 || ret == -5)
		return (ret);
	return (builtin_helper_v2(m, cmd));
}
