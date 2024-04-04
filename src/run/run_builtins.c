/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 02:36:00 by codespace         #+#    #+#             */
/*   Updated: 2024/04/04 03:04:05 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

int	resolve_builtin(t_env **env, t_cmd *cmd)
{
	int	ret;

	if (!cmd || !cmd->params || !cmd->params[0])
		return (0);
	ret = resolve_builtin_helper(env, cmd);
	if (ret != 0)
		return (ret);
	if (!ft_strncmp(cmd->params[0], "unset", -1))
		unset(env, cmd->params + 1);
	else if (!ft_strncmp(cmd->params[0], "export", -1)
		&& !export_cmd(env, cmd->params + 1))
		return (-1);
	else if (!ft_strncmp(cmd->params[0], "env", -1))
	{
		if (!print_env(*env))
			return (-1);
	}
	else if (!ft_strncmp(cmd->params[0], "pwd", -1)
		&& ft_printf("%s\n", get_var(*env, "PWD")) == -1)
		return (-1);
	else
		return (0);
	return (1);
}
