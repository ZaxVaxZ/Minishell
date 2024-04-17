/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 02:36:00 by codespace         #+#    #+#             */
/*   Updated: 2024/04/17 16:34:53 by pipolint         ###   ########.fr       */
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

	if (!child && (cmd->before == PIPE_OP || cmd->after == PIPE_OP))
		return (0);
	if (child && cmd->before != PIPE_OP && cmd->after != PIPE_OP)
		return (0);
	if (!cmd || !cmd->params || !cmd->params[0])
		return (0);
	ret = resolve_builtin_helper(env, cmd);
	if (ret == 1 || ret == -1)
		return (ret);
	if (!ft_strncmp(cmd->params[0], "unset", -1))
		unset(env, cmd->params + 1);
	else if (!ft_strncmp(cmd->params[0], "export", -1))
	{
		if (!export_cmd(env, cmd->params + 1))
			ret = -1;
	}
	else if (!ft_strncmp(cmd->params[0], "env", -1))
	{
		if (!print_env(*env))
			ret = -1;
	}
	else if (!ft_strncmp(cmd->params[0], "pwd", -1))
	{
		if (ft_printf("%s\n", get_var(*env, "PWD")) == -1)
			ret = -1;
	}
	else
		return (0);
	return (1);
}
