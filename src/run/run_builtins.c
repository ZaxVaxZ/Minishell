/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 02:36:00 by codespace         #+#    #+#             */
/*   Updated: 2024/04/15 19:04:34 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

static int	resolve_builtin_helper(t_env **env, t_cmd *cmd)
{
	char	*cwd;

	if (!ft_strncmp(cmd->params[0], "cd", -1))
	{
		cwd = getcwd(NULL, 0);
		if (!cd(env, cwd, cmd->params[1]))
		{
			free(cwd);
			return (-1);
		}
		free(cwd);
	}
	else if (!ft_strncmp(cmd->params[0], "echo", -1))
	{
		if (!echo(cmd->params + 1, (cmd->params[1]
					&& !ft_strncmp(cmd->params[1], "-n", 2))))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "exit", -1))
		return (-5);
	else
		return (0);
	return (1);
}

int	resolve_builtin(t_env **env, t_cmd *cmd, t_exec *exec, t_bool child)
{
	int	ret;

	if (!child && (exec->last_op == PIPE_OP || cmd->after == Op_pipe))
		return (0);
	if (child && exec->last_op != PIPE_OP && cmd->after != Op_pipe)
		return (0);
	if (!cmd || !cmd->params || !cmd->params[0])
		return (0);
	if (cmd->input && !cmd->heredoc)
	{
		cmd->in_fd = open(cmd->input, O_RDONLY, 0644);
		if (cmd->in_fd == -1)
		{
			perror(cmd->input);
			return (-1);
		}
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
		{
			perror(cmd->input);
			return (-1);
		}
	}
	if (cmd->outfile_cnt)
	{
		int i = 0;
		while (i < cmd->outfile_cnt)
		{
			if (cmd->out_flags[i])
				cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
			else
				cmd->out_fd = open(cmd->outfiles[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (cmd->out_fd < 0)
				perror(cmd->outfiles[cmd->outfile_cnt - 1]);
			printf("%s, %d\n", cmd->outfiles[i], cmd->out_fd);
			if (i < cmd->outfile_cnt - 1)
				close(cmd->out_fd);
			else if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			{
				perror(cmd->outfiles[cmd->outfile_cnt - 1]);
				return (-1);
			}
			i++;
		}
	}
	ret = resolve_builtin_helper(env, cmd);
	if (ret != 0)
	{
		if (cmd->in_fd != -1)
			close(cmd->in_fd);
		if (cmd->out_fd != -1)
			close(cmd->out_fd);
		return (ret);
	}
	if (!ft_strncmp(cmd->params[0], "unset", -1))
		unset(env, cmd->params + 1);
	else if (!ft_strncmp(cmd->params[0], "export", -1))
	{
		if (!export_cmd(env, cmd->params + 1))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "env", -1))
	{
		if (!print_env(*env))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "pwd", -1))
	{
		if (ft_printf("%s\n", get_var(*env, "PWD")) == -1)
			return (-1);
	}
	else
		return (0);
	return (1);
}
