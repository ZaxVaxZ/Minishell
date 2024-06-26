/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pipolint <pipolint@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 21:48:00 by marvin            #+#    #+#             */
/*   Updated: 2024/06/26 13:53:48 by pipolint         ###   ########.fr       */
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
		write(2, "exit: too many arguments\n", 25);
		if (cmd->before == PIPE_OP)
			exit(1);
		return (-2);
	}
	return (1);
}

int	exiting(t_cmd *cmd, char **params, t_exec *exec)
{
	int	code;
	int	i;

	if (!params)
		return (1);
	if (!*params)
		return (1);
	i = 0;
	while ((*params)[i])
	{
		if ((*params)[i] >= '0' && (*params)[i] <= '9')
			i++;
		else
			return (write_num_error(cmd, *params, exec, 0));
	}
	if (*(params + 1))
		return (write_num_error(cmd, *params, exec, 1));
	code = ft_atoi(*params);
	if (cmd->before == PIPE_OP)
		exit(code);
	(*exec->exit_status) = code;
	return (1);
}
