/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:48:00 by marvin            #+#    #+#             */
/*   Updated: 2024/07/25 11:05:51 by pipolint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	write_num_error(t_cmd *cmd, char *param, t_exec *exec, int reason)
{
	if (!reason)
	{
		if (write(2, "exit: ", 6) == -1)
			return (-1);
		if (write(2, param, ft_strlen(param)) == -1)
			return (-1);
		if (write(2, ": numeric argument required\n", 28) == -1)
			return (-1);
		if (cmd->before == PIPE_OP)
			exit(255);
		(*exec->exit_status) = 255;
	}
	else if (reason == 1)
	{
		if (write(2, "exit: too many arguments\n", 25) == -1)
			return (-1);
		if (cmd->before == PIPE_OP)
			exit(1);
		return (-2);
	}
	return (1);
}

void	exit_free(t_main *m, int code)
{
	free_queue(&m->q);
	free_cmd(&m->cmds);
	free_env(&m->env);
	free(m->line);
	exit(code);
}

int	exiting(t_main *m, t_cmd *cmd, char **params, t_exec *exec)
{
	int	code;
	int	sign;
	int	i;

	if (!params || !*params)
		return (1);
	i = 0;
	sign = 1;
	if ((*params)[i] == '-')
		sign *= -1;
	if ((*params)[i] == '+' || (*params)[i] == '-')
		i++;
	while ((*params)[i])
	{
		if (!((*params)[i] >= '0' && (*params)[i] <= '9'))
			return (write_num_error(cmd, *params, exec, 0));
		i++;
	}
	if (*(params + 1))
		return (write_num_error(cmd, *params, exec, 1));
	code = ft_atoi(*params);
	if (cmd->before == PIPE_OP)
		exit_free(m, code * sign);
	(*exec->exit_status) = code;
	return (1);
}
